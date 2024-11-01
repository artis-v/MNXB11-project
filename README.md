# MNXB11 Project by Artis Vijups and Sebastian Magnusson

# Report

The final report for this project can be found as `report.tex` and `MNXB11_Final_Project.pdf` within the `report` folder.

# About

In this project, the user provides a minimum and maximum temperature that establishes a range. Then, histograms are plotted summarizing three results:
* Each year, what was the _**count**_ of days in the range?
* Each year, what was the longest streak of days within the _**range**_?
* Each year, what was the longest streak of days within a range of the same _**amplitude**_?

We create a cubic polynomial fit and determine the mean and standard deviation for each of these three results.

# Building the project

Write `make` to build the project.

# Running the project

To run the project, run the `plot` BASH script by writing:

`./plot <minimum_temperature> <maximum_temperature> [<city> <time>]`

⚠️ This command **CLEARS** the `results` folder!

The command then fills the folder with new plots. Each plot is stored as both a `.tex` and a `.png` file.

If not specified, `<city>` is set to `Boras` and `<time>` is set to `18:00:00`. The variable `<city>` can be changed to anything so long as `datasets/<city>_Data_Cleaned.csv` exists.

Check the `datasets` folder to see the provided cleaned data and the `README` instructions for cleaning and adding more data.

# Viewing the results

By default, the `results` folder contains the plots created by running:

`./plot 16.6 18.1`

Other hand-picked interesting results are:
* `./plot -1.5 1.5` (see `range.png`),
* `./plot -1 1 Visby 12:00:00` (see `range.png`),
* `./plot -20 -10 Lulea` (see `count.png`),
* `./plot 0 0.5 Lulea 00:00:00` (see `amplitude.png`).