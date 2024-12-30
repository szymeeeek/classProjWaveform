#include <iostream>
#include <TH1F.h>
#include <string>
#include <sstream>
#include <fstream>
#include <TCanvas.h>
#include <vector>
#include <cmath>

using namespace std;

Bool_t scintilDet(std::string filename = "wave_0.dat", bool blSub = false) {
    Int_t nWaveforms = 0;
    const Int_t nBins = 1024;
    double minChannel = 0, maxChannel = nBins;

    float auTomV = 1 / 4096.0f; // mV

    fstream myfile;
    myfile.open(filename, ios::binary | ios::in);
    
    vector<float> counts;
    vector<float> error;
    float value;

    if (myfile.is_open()) {
        std::cout << filename << " opened successfully! :)" << std::endl;

        Int_t i = 0;
        while (!myfile.eof()) {
            myfile.read(reinterpret_cast<char*>(&value), sizeof(float));
            counts.push_back(value * auTomV);
            error.push_back(sqrt(counts.back()));
            i++;

            if (i < 1024) {
                std::cout << counts[i] << std::endl;
            }
        }
        myfile.close();
        
        Long_t nEntries = counts.size();
        std::cout << "Number of entries = " << nEntries << std::endl;
        nWaveforms = nEntries / nBins;
        std::cout << "Number of histograms = " << nWaveforms << std::endl;
    } else {
        std::cerr << "Something's wrong! " << filename << " couldn't be opened." << std::endl;
        return kFALSE;
    }

    Long64_t sum = 0;
    Double_t avg = 0;
    if (blSub) {
        for (Int_t k = 0; k < 50; k++) {
            sum = sum + counts[k];
        }
        avg = sum / 50;
        cout << "Subtracting " << avg << " baseline..." << endl;
    }

    Int_t j = 0;
    TH1F *hist = new TH1F(Form("waveform%i", 1), "", nBins, minChannel, maxChannel);
    
    // Loop over all waveforms
    for (Int_t i = 0; i < nWaveforms; i++) {
        // Fill the histogram with data for the current waveform
        for (Int_t j = i * nBins; j < (i + 1) * nBins; j++) {
            hist->SetBinContent(j - i * nBins + 1, counts[j] - avg);  
        }

        // Create and draw the canvas
        TCanvas *c1 = new TCanvas("c1", "waveform", 800, 600);
        c1->cd();
        hist->Draw("C");
        
        hist->GetXaxis()->SetTitle("time [ns]");
        hist->GetYaxis()->SetTitle("voltage [mV]");
        hist->GetYaxis()->SetTitleOffset(1.4);
        hist->GetXaxis()->SetTitleOffset(1.2);
        hist->SetStats(kFALSE);
        hist->SetLineColor(kBlue);
        hist->GetXaxis()->SetRangeUser(0, 1024);

        c1->Update();
        c1->WaitPrimitive();
    }

    return kTRUE;
}
