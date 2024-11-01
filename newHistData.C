#include <TAxis.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TGraph.h>
#include <TH1F.h>
#include <TLatex.h>
#include <TROOT.h>
#include <TString.h>
#include <TSystem.h>
#include <include/AnnualData.h>

#include <iostream>
#include <sstream>
#include <vector>

std::pair<Double_t, Double_t> calcMeanStdDev(const std::vector<Int_t> &values) {
    Double_t sum = 0.0, sumSq = 0.0;
    for (auto val : values) {
        sum += val;
        sumSq += val * val;
    }
    Double_t mean = sum / values.size();
    Double_t variance = (sumSq / values.size()) - (mean * mean);
    return std::make_pair(mean, std::sqrt(variance));
}

void drawHist(const std::vector<Int_t> &years, const std::vector<Int_t> &values,
              const std::string filename, const std::string title) {
    // Create canvas
    TCanvas *c = new TCanvas("c", "Yearly Data Histograms", 1200, 900);

    // Draw histogram
    c->cd();
    TH1F *hCounts =
        new TH1F("hCounts", (title + ";Year;Number of days").c_str(),
                 years.size(), years.front() - 0.5, years.back() + 0.5);
    for (size_t i = 0; i < years.size(); i++) {
        Int_t bin = hCounts->FindBin(years[i]);
        hCounts->SetBinContent(bin, values[i]);
    }
    hCounts->SetFillColor(kGreen - 9);
    hCounts->Draw();

    // Add fit
    TF1 *qFit = new TF1("qFit", "pol3", years.front(), years.back());
    hCounts->Fit(qFit, "RQ");
    qFit->SetLineColor(kRed);
    qFit->Draw("same");

    // Show mean and standard deviation
    auto [meanCounts, stdDevCounts] = calcMeanStdDev(values);
    TLatex stats;
    stats.SetNDC();
    stats.SetTextSize(0.03);
    stats.DrawLatex(0.75, 0.90, Form("Mean = %.2f", meanCounts));
    stats.DrawLatex(0.75, 0.85, Form("Std Dev = %.2f", stdDevCounts));
    c->Draw();

    // Write to files
    std::string texFile = "results/" + filename + ".tex";
    std::string pngFile = "results/" + filename + ".png";
    c->Print(texFile.c_str());
    c->Print(pngFile.c_str());

    // Clear
    delete qFit;
    delete hCounts;
    delete c;
}

void newHistData(const char *arg1, const char *arg2, const char *arg3,
                 const char *arg4) {
    // Check if the inputs are floats
    float min, max;
    try {
        min = std::stof(arg1);
        max = std::stof(arg2);
    } catch (...) {
        std::cerr << "Error: Inputs must be numbers." << std::endl;
        return;
    }

    // Check order
    if (max < min) {
        std::cerr << "Error: Lower temperature must be first." << std::endl;
        return;
    }

    // Load shared object
    gSystem->Load("AnnualData.so");

    // Get vectors using the class directly
    AnnualData ad(arg3, arg4);
    std::vector<Int_t> years = ad.years();
    std::vector<Int_t> counts = ad.count(min, max);
    std::vector<Int_t> ranges = ad.range(min, max);
    std::vector<Int_t> amplitudes = ad.amplitude(max - min);

    // Check that all data vectors are the same size
    if (years.size() != counts.size() || years.size() != ranges.size() ||
        years.size() != amplitudes.size()) {
        std::cerr << "Error: Data vectors have mismatched sizes!" << std::endl;
        return;
    }

    // Never display canvas windows
    gROOT->SetBatch(kTRUE);

    // Draw histograms
    drawHist(years, counts, "count",
             Form("\\text{Number of days between }"
                  "%.1f^{\\circ}C\\text{ and }"
                  "%.1f^{\\circ}C\\text{ by year}",
                  min, max));
    drawHist(years, ranges, "range",
             Form("\\text{Longest streak of days between }"
                  "%.1f^{\\circ}C\\text{ and }"
                  "%.1f^{\\circ}C\\text{ by year}",
                  min, max));
    drawHist(years, amplitudes, "amplitude",
             Form("\\text{Longest streak of days within a }"
                  "%.1f^{\\circ}C\\text{ range by year}",
                  max - min));
}
