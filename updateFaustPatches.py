import subprocess, glob

faustFiles = glob.glob("*.dsp")

faustFiles.remove("IIRHilbert.dsp")
faustFiles.remove("FrequencyShifter.dsp")

for file in faustFiles:
    command = ["faust2owl", file]
    subprocess.call(command)