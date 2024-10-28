#include <TSystem.h>
#include <TString.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <iostream>
#include <sstream>
#include <vector>

// Make sure the c++ code is compiled and has the executable called "main"
// Else change the path below

void plotData() {
    // Run the main executable and capture its output
    TString cmd = "./main";  // Path to the compiled executable
    TString output = gSystem->GetFromPipe(cmd);  // Capture the output of the executable

    // Split the output by lines and process each line
    std::istringstream stream(output.Data());
    std::string line;
    std::vector<Double_t> years, counts, ranges, amplitudes;
    int lineNum = 0;

    // Parse each line into separate vectors
    while (std::getline(stream, line)) {
        std::istringstream lineStream(line);
        Double_t value;
        
        if (lineNum == 0) {
            while (lineStream >> value) years.push_back(value);
        } else if (lineNum == 1) {
            while (lineStream >> value) counts.push_back(value);
        } else if (lineNum == 2) {
            while (lineStream >> value) ranges.push_back(value);
        } else if (lineNum == 3) {
            while (lineStream >> value) amplitudes.push_back(value);
        }
        lineNum++;
    }

    // Check that the years vector has the same size as counts, ranges, and amplitudes
    if (years.size() != counts.size() || years.size() != ranges.size() || years.size() != amplitudes.size()) {
        std::cerr << "Error: Data vectors have mismatched sizes!" << std::endl;
        return;
    }

    // Create a canvas for the plots
    TCanvas *c1 = new TCanvas("c1", "Yearly Data Plots", 1200, 900);
    c1->Divide(2, 2);

    // Create and draw TGraphs with years on the x-axis for each data set
    // Counts graph
    c1->cd(1);
    TGraph *gCounts = new TGraph(years.size(), &years[0], &counts[0]);
    gCounts->SetTitle("Counts over Years;Year;Counts");
    gCounts->SetMarkerStyle(20);
    gCounts->Draw("APL");

    // Ranges graph
    c1->cd(2);
    TGraph *gRanges = new TGraph(years.size(), &years[0], &ranges[0]);
    gRanges->SetTitle("Ranges over Years;Year;Ranges");
    gRanges->SetMarkerStyle(21);
    gRanges->Draw("APL");

    // Amplitudes graph
    c1->cd(3);
    TGraph *gAmplitudes = new TGraph(years.size(), &years[0], &amplitudes[0]);
    gAmplitudes->SetTitle("Amplitudes over Years;Year;Amplitudes");
    gAmplitudes->SetMarkerStyle(22);
    gAmplitudes->Draw("APL");

    // Update the canvas to display all graphs
    c1->Update();
}
