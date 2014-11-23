import subprocess, glob, shutil

vstdir = "/Library/Audio/Plug-Ins/VST/MyFaust"

faustFiles = glob.glob("*.dsp")

for file in faustFiles:
  if(file != "IIRHilbert.dsp"):
    command = ["faust2vst", file]
    subprocess.call(command)
    
vstFiles = glob.glob("*.vst")

for file in vstFiles:
  outputfile = vstdir + "/" + file
  shutil.rmtree(outputfile)
  shutil.move(file, outputfile)