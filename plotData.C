#include <TSystem.h>
#include <TString.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <include/AnnualData.h>

// Make sure the c++ code is compiled and has the executable called "main"
// Else change the path below

void plotData() {
    // Load shared object
    gSystem->Load("AnnualData.so");

    // Get vectors using the class directly
    AnnualData ad("datasets/Boras_Data_Cleaned.csv", "18:00:00");
    std::vector<Int_t> years = ad.years();
    std::vector<Int_t> counts = ad.count(16.6, 18.1);
    std::vector<Int_t> ranges = ad.count(16.6, 18.1);
    std::vector<Int_t> amplitudes = ad.count(16.6, 18.1);
    
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
