/*
// Jeffrey Coughlin
// jeffrey.l.coughlin@nasa.gov
// October 5, 2015
//
// Generate-FPP-PDFs-V1.cpp
// Program to make the FPP PDFs
//
// Compile via:
// g++ -o generate Generate-FPP-PDFs-V1.cpp
// And simply run as:
// ./generate KOIS.txt
// where KOIS.txt is a list of all KOIs you want to generate, one per line, in the format KXXXXX.YY, e.g., K07016.01.
//
// You must have pdflatex installed on the system, along with the overpix, geometry, and hyperref packages, (which should be standard). You might get some latex overfull hbox warnings - just ignore them.
//
// The program assumes all the PNGs for each KOI are in the subdirectory ./PDFS/KXXXXX.YY/, e.g., ./PDFS/KOI07016.01/
// You can change this on line 45 if needed
//
*/


// Declare Headers
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>

// Declare constants
const int N=10000;   // Max number of KOIs allowed. Increase if needed

using namespace std;

// Quick function to check if a file exists
bool fexists(const char *filename)
  {
  ifstream ifile(filename);
  return ifile;
  }


int main (int argc, char* argv[]) {

  
// CHANGE PNG DIRECTORY HERE!!!
string PDFDIR = "./PNGS/";  // Name of directory that contains all the PNG files, grouped by KOI. E.g.,  ./PNGS/K07016.01/ is where the PNGS are for 7016.01, so PDFDIR is "./PNGS/"

// Declare other variables
int i;  // General counting integer
ifstream input;  // Input TCE list variable
string infilename;  // File name of input file
ofstream texout;  // Output variable for generating latex files
string koiname[N],pdfoutname,syscmd,tmpstr1;  // Array of KOI names, name of the output tex/pdf file, string to execute a system command, and a temp string
int nkoi;

  
// Get input filename from command line, else prompt for it
if(argc>1)
  infilename = argv[1];
else
  {
  cout << "Name of Input File of KOIs?: ";
  cin >> infilename;
  }
  

input.open(infilename.c_str());  // Open up the list of FPP KOIs for which to make the PDFs. It should just contain the KOI number, e.g., K07016.01, one per line, in the order you want them produced.

i=0;  // Now we read in the input list
input >> koiname[i];
while(!input.eof())
  {
  i++;
  input >> koiname[i];
  }
nkoi=i;  // Record the total number of KOIs
input.close();


if(nkoi==0)
  {
  cout << "Input List is Empty! Exiting..." << endl;
  exit(0);
  }


i=0;
while(i<nkoi)  // Loop over all KOI's
  {
  pdfoutname = koiname[i] + ".tex";  // The name of the tex/pdf file
  texout.open(pdfoutname.c_str());  // Open the tex file for writing 
  
  
  texout  << "\\documentclass[letterpaper,10pt]{article}" << endl  // Output all the tex to the file
          << "\\usepackage[bookmarks=true]{hyperref}" << endl      // Enable PDF bookmarks
          << "\\usepackage[landscape,left=0.0in,right=0in,top=0.0in,bottom=0in,noheadfoot]{geometry}" << endl  // Initialize geometry
          << "\\usepackage[abs]{overpic}" << endl  // Main package for overlaying plots and text
          << "\\begin{document}" << endl   // Begin doc
          << "\\pagestyle{empty}" << endl;  // empty pages to put plots on
          

// Summary page          
  texout
          << "\\pdfbookmark[0]{Summary}{Summary}" << endl
          << "\\newgeometry{left=0.25in,right=0.05in,top=0.15in,bottom=0.15in,noheadfoot} " << endl
          << "\\begin{tabular*}{11in}{cc}" << endl;
          tmpstr1 = PDFDIR + koiname[i] + "/FPPsummary.png";
          if(fexists(tmpstr1.c_str()))
            texout << "\\begin{overpic}[width=5.25in,height=4.5in,keepaspectratio,tics=25,trim=0 0 0 0,clip]{" << tmpstr1 << "}" << endl;
          else
            texout << "\\begin{overpic}[width=5.25in,tics=20,trim=0 0 0 0,clip]{NoPlot.png}" << endl;
          texout << "\\end{overpic} & " << endl;
          
          tmpstr1 = PDFDIR + koiname[i] + "/signal.png";
          if(fexists(tmpstr1.c_str()))
            texout << "\\begin{overpic}[width=5.25in,height=4.5in,keepaspectratio,tics=25,trim=0 0 0 0,clip]{" << tmpstr1 << "}" << endl;
          else
            texout << "\\begin{overpic}[width=5.25in,tics=20,trim=0 0 0 0,clip]{NoPlot.png}" << endl;
          texout << "\\end{overpic} \\\\ " << endl;
          
          texout << "\\parbox{5.25in}{\\vspace{-4in}\\LARGE Summary plots of the `vespa' calculation.  Top-left shows the values of the prior and likelihood factors for each scenario. Top-right shows the phased transit light curve used and the best-fit trapezoid model.  Bottom-right shows the trapezoid shape parameter likelihood space for the transiting planet model.  The red point + error bar represents the uncertainty on the trapezoidal shape parameters for the signal.} & " << endl;

          tmpstr1 = PDFDIR + koiname[i] + "/pl.png";
          if(fexists(tmpstr1.c_str()))
            texout << "\\begin{overpic}[width=5.25in,height=4.5in,keepaspectratio,tics=25,trim=0 0 0 0,clip]{" << tmpstr1 << "}" << endl;
          else
            texout << "\\begin{overpic}[width=5.25in,tics=20,trim=0 0 0 0,clip]{NoPlot.png}" << endl;
          texout << "\\end{overpic}" << endl
                 << "\\end{tabular*}" << endl
                 << "\\clearpage" << endl;
                      
// EB Fractions                 
  texout
          << "\\pdfbookmark[0]{EB Probabilities}{EB Probabilities}" << endl
          << "\\newgeometry{left=0.25in,right=0.05in,top=0.15in,bottom=0.15in,noheadfoot} " << endl
          << "\\begin{tabular*}{11in}{cc}" << endl;
          tmpstr1 = PDFDIR + koiname[i] + "/eb.png";
          if(fexists(tmpstr1.c_str()))
            texout << "\\begin{overpic}[width=5.25in,height=4.5in,keepaspectratio,tics=25,trim=0 0 0 0,clip]{" << tmpstr1 << "}" << endl;
          else
            texout << "\\begin{overpic}[width=5.25in,tics=20,trim=0 0 0 0,clip]{NoPlot.png}" << endl;
          texout << "\\end{overpic} & " << endl;
          
          tmpstr1 = PDFDIR + koiname[i] + "/heb.png";
          if(fexists(tmpstr1.c_str()))
            texout << "\\begin{overpic}[width=5.25in,height=4.5in,keepaspectratio,tics=25,trim=0 0 0 0,clip]{" << tmpstr1 << "}" << endl;
          else
            texout << "\\begin{overpic}[width=5.25in,tics=20,trim=0 0 0 0,clip]{NoPlot.png}" << endl;
          texout << "\\end{overpic} \\\\ " << endl;
          
          texout << "\\parbox{5.25in}{\\vspace{-4in}\\LARGE Trapezoid shape parameter likelihood surface plots for the false positive scenarios considered by `vespa'. EB = undiluted eclipsing binary, HEB = hierarchical triple eclipsing binary, and BEB = blended eclipsing binary (chance-aligned).  The red ‘x’ and error bars represent the shape of the observed signal.} & " << endl;

          tmpstr1 = PDFDIR + koiname[i] + "/beb.png";
          if(fexists(tmpstr1.c_str()))
            texout << "\\begin{overpic}[width=5.25in,height=4.5in,keepaspectratio,tics=25,trim=0 0 0 0,clip]{" << tmpstr1 << "}" << endl;
          else
            texout << "\\begin{overpic}[width=5.25in,tics=20,trim=0 0 0 0,clip]{NoPlot.png}" << endl;
          texout << "\\end{overpic}" << endl
                 << "\\end{tabular*}" << endl
                 << "\\clearpage" << endl;
       
                 
// EB Double period                 
  texout
          << "\\pdfbookmark[0]{Double Periods}{Double Periods}" << endl
          << "\\newgeometry{left=0.25in,right=0.05in,top=0.15in,bottom=0.15in,noheadfoot} " << endl
          << "\\begin{tabular*}{11in}{cc}" << endl;
          tmpstr1 = PDFDIR + koiname[i] + "/eb_Px2.png";
          if(fexists(tmpstr1.c_str()))
            texout << "\\begin{overpic}[width=5.25in,height=4.5in,keepaspectratio,tics=25,trim=0 0 0 0,clip]{" << tmpstr1 << "}" << endl;
          else
            texout << "\\begin{overpic}[width=5.25in,tics=20,trim=0 0 0 0,clip]{NoPlot.png}" << endl;
          texout << "\\end{overpic} & " << endl;
          
          tmpstr1 = PDFDIR + koiname[i] + "/heb_Px2.png";
          if(fexists(tmpstr1.c_str()))
            texout << "\\begin{overpic}[width=5.25in,height=4.5in,keepaspectratio,tics=25,trim=0 0 0 0,clip]{" << tmpstr1 << "}" << endl;
          else
            texout << "\\begin{overpic}[width=5.25in,tics=20,trim=0 0 0 0,clip]{NoPlot.png}" << endl;
          texout << "\\end{overpic} \\\\ " << endl;
          
          texout << "\\parbox{5.25in}{\\vspace{-4in}\\LARGE Trapezoid shape parameter likelihood surface plots for the false positive scenarios considered by `vespa', specifically for a scenario where the KOI is detected at half the true orbital period. EB = undiluted eclipsing binary, HEB = hierarchical triple eclipsing binary, and BEB = blended eclipsing binary (chance-aligned).  The red ‘x’ and error bars represent the shape of the observed signal.} & " << endl;

          tmpstr1 = PDFDIR + koiname[i] + "/beb_Px2.png";
          if(fexists(tmpstr1.c_str()))
            texout << "\\begin{overpic}[width=5.25in,height=4.5in,keepaspectratio,tics=25,trim=0 0 0 0,clip]{" << tmpstr1 << "}" << endl;
          else
            texout << "\\begin{overpic}[width=5.25in,tics=20,trim=0 0 0 0,clip]{NoPlot.png}" << endl;
          texout << "\\end{overpic}" << endl
                 << "\\end{tabular*}" << endl
                 << "\\clearpage" << endl;
                 
                 
// First single triangle plot
  texout
          << "\\pdfbookmark[0]{Single Star Isochrones}{Single Star Isochrones}" << endl
          << "\\newgeometry{left=0.25in,right=0.25in,top=0.25in,bottom=0.25in,noheadfoot}" << endl;

          tmpstr1 = PDFDIR + koiname[i] + "/dartmouth_triangle_single_physical.png";
          if(fexists(tmpstr1.c_str()))
            texout
            << "\\begin{overpic}[width=8.0in,height=9.5in,keepaspectratio,tics=20,trim=0 0 0 0,clip]{" << tmpstr1 << "}" << endl;
          else
            texout
            << "\\begin{overpic}[width=8.0in,tics=20,trim=0 0 0 0,clip]{NoPlot.png}" << endl
            << "\\put(180,495){\\LARGE Results of the single-star ‘isochrones’ stellar parameter fits.}" << endl;
          texout
            << "\\put(180,495){\\LARGE Results of the single-star ‘isochrones’ stellar parameter fits.}" << endl
            << "\\end{overpic}" << endl
            << "\\clearpage" << endl;
            
            
// Second single triangle plot
  texout
          << "\\pdfbookmark[0]{Single Star Correlation}{Single Star Correlation}" << endl
          << "\\newgeometry{left=0.25in,right=0.25in,top=0.25in,bottom=0.25in,noheadfoot}" << endl;

          tmpstr1 = PDFDIR + koiname[i] + "/dartmouth_triangle_single_observed.png";
          if(fexists(tmpstr1.c_str()))
            texout
            << "\\begin{overpic}[width=8.0in,height=9.5in,keepaspectratio,tics=20,trim=0 0 0 0,clip]{" << tmpstr1 << "}" << endl;
          else
            texout
            << "\\begin{overpic}[width=8.0in,tics=20,trim=0 0 0 0,clip]{NoPlot.png}" << endl
            << "\\put(200,485){\\LARGE Correlation plot of the constraints used in the single-star `isochrones' stellar}" << endl
            << "\\put(200,465){\\LARGE parameter fits. The blue lines represent the observed values for each quantity.}" << endl;
          texout
            << "\\put(200,485){\\LARGE Correlation plot of the constraints used in the single-star `isochrones' stellar}" << endl
            << "\\put(200,465){\\LARGE parameter fits. The blue lines represent the observed values for each quantity.}" << endl
            << "\\end{overpic}" << endl
            << "\\clearpage" << endl;
            
    
// First binary triangle plot
  texout
          << "\\pdfbookmark[0]{Binary Star Isochrones}{Binary Star Isochrones}" << endl
          << "\\newgeometry{left=0.25in,right=0.25in,top=0.25in,bottom=0.25in,noheadfoot}" << endl;

          tmpstr1 = PDFDIR + koiname[i] + "/dartmouth_triangle_binary_physical.png";
          if(fexists(tmpstr1.c_str()))
            texout
            << "\\begin{overpic}[width=8.0in,height=9.5in,keepaspectratio,tics=20,trim=0 0 0 0,clip]{" << tmpstr1 << "}" << endl;
          else
            texout
            << "\\begin{overpic}[width=8.0in,tics=20,trim=0 0 0 0,clip]{NoPlot.png}" << endl
            << "\\put(165,500){\\LARGE Results of the binary-star ‘isochrones’ stellar parameter fits.}" << endl;
          texout
            << "\\put(165,500){\\LARGE Results of the binary-star ‘isochrones’ stellar parameter fits.}" << endl
            << "\\end{overpic}" << endl
            << "\\clearpage" << endl;
            
            
// Second binary triangle plot
  texout
          << "\\pdfbookmark[0]{Binary Star Correlation}{Binary Star Correlation}" << endl
          << "\\newgeometry{left=0.25in,right=0.25in,top=0.25in,bottom=0.25in,noheadfoot}" << endl;

          tmpstr1 = PDFDIR + koiname[i] + "/dartmouth_triangle_binary_observed.png";
          if(fexists(tmpstr1.c_str()))
            texout
            << "\\begin{overpic}[width=8.0in,height=9.5in,keepaspectratio,tics=20,trim=0 0 0 0,clip]{" << tmpstr1 << "}" << endl;
          else
            texout
            << "\\begin{overpic}[width=8.0in,tics=20,trim=0 0 0 0,clip]{NoPlot.png}" << endl
            << "\\put(200,485){\\LARGE Correlation plot of the constraints used in the binary-star `isochrones' stellar}" << endl
            << "\\put(200,465){\\LARGE parameter fits. The blue lines represent the observed values for each quantity.}" << endl;
          texout
            << "\\put(200,485){\\LARGE Correlation plot of the constraints used in the binary-star `isochrones' stellar}" << endl
            << "\\put(200,465){\\LARGE parameter fits. The blue lines represent the observed values for each quantity.}" << endl
            << "\\end{overpic}" << endl
            << "\\clearpage" << endl;
           
            
// First triple triangle plot
  texout
          << "\\pdfbookmark[0]{Triple Star Isochrones}{Triple Star Isochrones}" << endl
          << "\\newgeometry{left=0.25in,right=0.25in,top=0.25in,bottom=0.25in,noheadfoot}" << endl;

          tmpstr1 = PDFDIR + koiname[i] + "/dartmouth_triangle_triple_physical.png";
          if(fexists(tmpstr1.c_str()))
            texout
            << "\\begin{overpic}[width=8.0in,height=9.5in,keepaspectratio,tics=20,trim=0 0 0 0,clip]{" << tmpstr1 << "}" << endl;
          else
            texout
            << "\\begin{overpic}[width=8.0in,tics=20,trim=0 0 0 0,clip]{NoPlot.png}" << endl
            << "\\put(150,510){\\LARGE Results of the triple-star ‘isochrones’ stellar parameter fits.}" << endl;
          texout
            << "\\put(150,510){\\LARGE Results of the triple-star ‘isochrones’ stellar parameter fits.}" << endl
            << "\\end{overpic}" << endl
            << "\\clearpage" << endl;
            
            
// Second triple triangle plot
  texout
          << "\\pdfbookmark[0]{Triple Star Correlation}{Triple Star Correlation}" << endl
          << "\\newgeometry{left=0.25in,right=0.25in,top=0.25in,bottom=0.25in,noheadfoot}" << endl;

          tmpstr1 = PDFDIR + koiname[i] + "/dartmouth_triangle_triple_observed.png";
          if(fexists(tmpstr1.c_str()))
            texout
            << "\\begin{overpic}[width=8.0in,height=9.5in,keepaspectratio,tics=20,trim=0 0 0 0,clip]{" << tmpstr1 << "}" << endl;
          else
            texout
            << "\\begin{overpic}[width=8.0in,tics=20,trim=0 0 0 0,clip]{NoPlot.png}" << endl
            << "\\put(200,485){\\LARGE Correlation plot of the constraints used in the triple-star `isochrones' stellar}" << endl
            << "\\put(200,465){\\LARGE parameter fits. The blue lines represent the observed values for each quantity.}" << endl;
          texout
            << "\\put(200,485){\\LARGE Correlation plot of the constraints used in the triple-star `isochrones' stellar}" << endl
            << "\\put(200,465){\\LARGE parameter fits. The blue lines represent the observed values for each quantity.}" << endl
            << "\\end{overpic}" << endl
            << "\\clearpage" << endl;
           

  texout  << "\\end{document}" << endl;  // End document
  texout.close();  // Close tex file
  
  // Now run pdflatex and make PDF
  syscmd = "pdflatex " + pdfoutname;  // The command we're going to execute - pdflatex pdfoutname.tex
  system(syscmd.c_str());   // Execute pdflatex to build the pdf file
  system(syscmd.c_str());   // Gotta do it twice for the hyperlinks to be created and then updated
  
  // And clean up non-PDF files
  syscmd = "rm " + koiname[i] + ".tex";
  system(syscmd.c_str());
  
  syscmd = "rm " + koiname[i] + ".out";
  system(syscmd.c_str());
  
  syscmd = "rm " + koiname[i] + ".aux";
  system(syscmd.c_str());
  
  syscmd = "rm " + koiname[i] + ".log";
  system(syscmd.c_str());
  
  
  i++;  // Go to next KOI
  }
}

