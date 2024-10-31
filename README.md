# MNXB11 Project by Artis Vijups and Sebastian Magnusson

# About

In this project, the user provides a minimum and maximum temperature that establishes a range. Then, histograms are plotted summarizing three results:
* Each year, what was the **count** of days in the range?
* Each year, what was the longest streak of days within the **range**?
* Each year, what was the longest streak of days within a range of the same **amplitude**?

We create a fit and determine the mean and standard deviation for each of these three results.

# Building the project

Write `make` to build the project.

# Run the project

To run the project, run the `plot` BASH script by writing:

`./plot <minimum_temperature> <maximum_temperature>`

⚠️ This command **CLEARS** the `results` folder!

The command then fills the folder with new plots. Each plot is stored as both a `.tex` and a `.png` file. By default, this folder contains the plots created by running:

`./plot 16.6 18.1`