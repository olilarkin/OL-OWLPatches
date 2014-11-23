import subprocess, glob, shutil, os.path

vstdir = "/Library/Audio/Plug-Ins/VST/MyFaust"

faustFiles = glob.glob("*.dsp")

for file in faustFiles:
  if(file != "IIRHilbert.dsp" || file != "FrequencyShifter.dsp"):
    command = ["faust2vst", file]
    subprocess.call(command)
    
vstFiles = glob.glob("*.vst")

for file in vstFiles:
  outputfile = vstdir + "/" + file
  if os.path.isdir(outputfile):
    shutil.rmtree(outputfile)
  shutil.move(file, outputfile)