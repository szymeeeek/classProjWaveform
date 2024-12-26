#include <iostream>
#include <TH1F.h>
#include <string>
#include <sstream>
#include <fstream>
#include <TH1F.h>
#include <TCanvas.h>
#include <vector>
using namespace std;

Bool_t scintilDet(std::string filename = "wave0.dat", bool blSub = false){
    Int_t nWaveforms = 4;
    const Int_t nBins = 1024;
    double minChannel = 0, maxChannel = nBins;

    Double_t auTomV = 1/4096;

    TH1F *hist[4] = {nullptr};

    fstream myfile;
    myfile.open(filename, ios::binary | ios::in);
    
    vector <double> counts;
    vector <double> error;

    if(myfile.is_open()){
        std::cout<<filename<<" opened successfully! :)"<<std::endl;

        Int_t i = 0;
        while(!myfile.eof()){
            myfile.read((char*)&counts[i], sizeof(Double_t));
            counts[i] = counts[i]*auTomV;
            error[i] = sqrt(counts[i]);
            i++;
        }
        myfile.close();
    }
    else{
        std::cerr<<"Something's wrong! "<<filename<<" coudn't be opened."<<std::endl;
        return kFALSE;
    }

    Long64_t sum = 0;
    Double_t avg = 0;
    if(blSub){
        for(Int_t k = 0; k < 50; k++){
            sum = sum + counts[k];
        }
        avg = sum/50;

        cout<<"subtracting "<<avg<<" baseline..."<<endl;
    }

    for(Int_t i = 0; i < nWaveforms; i++){
        //create a histo here
        hist[i] = new TH1F("", "", nBins, minChannel, maxChannel);

        for(Int_t j = 0; j<nBins; j++){
            //fill the histo here
            hist[i]->SetBinContent(j, counts[j]-avg);
            hist[i]->SetBinError(j, error[j]);
        }
        TCanvas *c1 = new TCanvas("c1", "waveform", 800, 600);
        c1->cd();
        hist[i]->Draw();
        hist[i]->GetXaxis()->SetTitle("time [ns]");
        hist[i]->GetYaxis()->SetTitle("voltage [not yet V]");
        hist[i]->GetYaxis()->SetTitleOffset(1.4);
        hist[i]->GetXaxis()->SetTitleOffset(1.2);
        hist[i]->SetTitle(Form("waveform%i", i));
        hist[i]->SetStats(kFALSE);
        hist[i]->SetLineColor(kBlue);
        hist[i]->SetMarkerStyle(20);
        hist[i]->SetMarkerSize(0.5);
        hist[i]->SetMarkerColor(kRed);
        hist[i]->GetXaxis()->SetRangeUser(0, 1024);
        c1->WaitPrimitive();
    }
    return kTRUE;
}