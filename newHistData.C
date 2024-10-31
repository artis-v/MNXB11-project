#include <TAxis.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TGraph.h>
#include <TH1F.h>
#include <TLatex.h>
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

void newHistData(const char *arg1, const char *arg2) {
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
    AnnualData ad("datasets/Boras_Data_Cleaned.csv", "18:00:00");
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

    drawHist(years, counts, "count",
             Form("\\text{Number of days between }%.1f^{\\circ}C\\text{ and }%.1f^{\\circ}C\\text{ by year}", min, max));
    drawHist(years, ranges, "range",
             Form("\\text{Longest streak of days between }%.1f^{\\circ}C\\text{ and }%.1f^{\\circ}C\\text{ by year}", min, max));
    drawHist(years, amplitudes, "amplitude",
             Form("\\text{Longest streak of days within a }%.1f^{\\circ}C\\text{ range by year}", max - min));

    /*

    // Determine histogram range based on the years vector
    Int_t yearMin = *std::min_element(years.begin(), years.end());
    Int_t yearMax = *std::max_element(years.begin(), years.end());
    Int_t nBins = years.size();

    // Create a canvas for the histograms
    TCanvas *c = new TCanvas("c", "Yearly Data Histograms", 1200, 900);
    // c->Divide(2, 2);

    auto calcMeanStdDev = [](const std::vector<Int_t> &values) {
        Double_t sum = 0.0, sumSq = 0.0;
        for (auto val : values) {
            sum += val;
            sumSq += val * val;
        }
        Double_t mean = sum / values.size();
        Double_t variance = (sumSq / values.size()) - (mean * mean);
        return std::make_pair(mean, std::sqrt(variance));
    };

    // Counts histogram
    c->cd();
    TH1F *hCounts = new TH1F("hCounts", "Counts over Years;Year;Counts", nBins,
                             years.front() - 0.5, years.back() + 0.5);
    for (size_t i = 0; i < years.size(); i++) {
        Int_t bin = hCounts->FindBin(years[i]);
        hCounts->SetBinContent(bin, counts[i]);
    }
    hCounts->SetFillColor(kGreen - 9);
    hCounts->Draw();

    TF1 *qFit = new TF1("qFit", "pol3", years.front(), years.back());
    hCounts->Fit(qFit, "R");
    qFit->SetLineColor(kRed);
    qFit->Draw("same");

        Double_t chi2_counts = qFit->GetChisquare();
        Int_t ndf_counts = qFit->GetNDF();
        TLatex latex;
        latex.SetTextSize(0.03);
        latex.DrawLatexNDC(0.75, 0.65, Form("#frac{#chi^{2}}{ndf} = %.3f",
       chi2_counts / ndf_counts));


    auto [meanCounts, stdDevCounts] = calcMeanStdDev(counts);
    TLatex stats1;
    stats1.SetNDC();
    stats1.SetTextSize(0.03);
    stats1.DrawLatex(0.75, 0.90, Form("Mean = %.2f", meanCounts));
    stats1.DrawLatex(0.75, 0.85, Form("Std Dev = %.2f", stdDevCounts));
    c->Draw();
    c->Print("ROOT/c.tex");
    c->Print("ROOT/c.png");

    // Ranges histogram
    TCanvas *c2 = new TCanvas("c2", "Yearly Data Histograms", 1200, 900);
    c2->cd();

    TH1F *hRanges = new TH1F("hRanges", "Ranges over Years;Year;Ranges", nBins,
                             yearMin - 0.5, yearMax + 0.5);
    for (size_t i = 0; i < years.size(); i++) {
        Int_t bin = hRanges->FindBin(years[i]);
        hRanges->SetBinContent(bin, ranges[i]);
    }
    hRanges->SetFillColor(kGreen - 9);
    hRanges->Draw();

    TF1 *qFit2 = new TF1("qFit2", "pol3", yearMin, yearMax);
    hRanges->Fit(qFit2, "Q");
    qFit2->SetLineColor(kRed);
    qFit2->Draw("same");

    auto [meanRanges, stdDevRanges] = calcMeanStdDev(ranges);
    TLatex stats2;
    stats2.SetNDC();
    stats2.SetTextSize(0.03);
    stats2.DrawLatex(0.725, 0.90, Form("Mean = %.2f", meanRanges));
    stats2.DrawLatex(0.725, 0.85, Form("Std Dev = %.2f", stdDevRanges));
    // Amplitudes histogram
    c2->Draw();
    c2->Print("ROOT/c2.tex");
    c2->Print("ROOT/c2.png");
    TCanvas *c3 = new TCanvas("c3", "Yearly Data Histograms", 1200, 900);
    c3->cd();

    TH1F *hAmplitudes =
        new TH1F("hAmplitudes", "Amplitudes over Years;Year;Amplitudes", nBins,
                 yearMin - 0.5, yearMax + 0.5);
    for (size_t i = 0; i < years.size(); i++) {
        Int_t bin = hAmplitudes->FindBin(years[i]);
        hAmplitudes->SetBinContent(bin, amplitudes[i]);
    }
    hAmplitudes->SetFillColor(kGreen - 9);
    hAmplitudes->Draw();

    TF1 *qFit3 = new TF1("qFit2", "pol3", yearMin, yearMax);
    hAmplitudes->Fit(qFit3, "Q");
    qFit3->SetLineColor(kRed);
    qFit3->Draw("same");

    auto [meanAmplitudes, stdDevAmplitudes] = calcMeanStdDev(amplitudes);
    TLatex stats3;
    stats3.SetNDC();
    stats3.SetTextSize(0.03);
    stats3.DrawLatex(0.75, 0.90, Form("Mean = %.2f", meanAmplitudes));
    stats3.DrawLatex(0.75, 0.85, Form("Std Dev = %.2f", stdDevAmplitudes));

    c3->Draw();

    c3->Print("ROOT/c3.tex");
    c3->Print("ROOT/c3.png");
    */
    // f1->Close();
}
