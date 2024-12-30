# ScintilDet Macro README

## Overview
The `scintilDet` macro is designed to read binary waveform data from a specified file and process it into histograms using ROOT framework. The macro supports optional baseline subtraction and generates visual histograms for each waveform.

## Features
- **File Reading**: The macro reads binary files containing waveform data.
- **Data Conversion**: Data is converted from raw counts to millivolts (mV).
- **Baseline Subtraction**: Optionally subtracts the baseline value from the waveform data.
- **Histogram Generation**: Creates a histogram for each waveform, visualizing the time versus voltage (mV).
- **Error Calculation**: Computes the error for each waveform point based on the square root of the count.
- **Canvas Output**: Displays each histogram on a ROOT canvas.

## Usage

1. **Include ROOT Headers**: The macro requires ROOT libraries such as `TH1F`, `TCanvas`, and `fstream`.

2. **Function Declaration**:
   - The function `scintilDet()` is defined as follows:
     ```cpp
     Bool_t scintilDet(std::string filename = "wave_0.dat", bool blSub = false);
     ```
   - **Parameters**:
     - `filename` (optional): The input binary file containing the waveform data (default is `"wave_0.dat"`).
     - `blSub` (optional): A boolean flag to enable baseline subtraction (default is `false`).

3. **How It Works**:
   - The macro reads the binary file `wave_0.dat` (or a user-specified file) to extract waveform data.
   - The data is converted to mV using the `1/4096` scaling factor.
   - Optionally, a baseline subtraction can be performed by averaging the first 50 data points.
   - Histograms are created and drawn for each waveform, showing the voltage (mV) against time (ns).
   - The histograms are displayed on ROOT canvases.

## Example

To call the `scintilDet` macro in your ROOT environment, run the following:

```cpp
scintilDet("waveform_data.dat", true);
