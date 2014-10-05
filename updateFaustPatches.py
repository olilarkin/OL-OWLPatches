import subprocess, glob

faustFiles = glob.glob("*.dsp")

for file in faustFiles:
  if(file != "IIRHilbert.dsp"):
    command = ["faust2owl", file]
    subprocess.call(command)