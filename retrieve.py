import os
from shutil import copyfile

project_source = os.path.normpath(os.path.abspath("../raytraceproject/raytraceproject"))

print("working...")
#single level project for now
extensions = ["h","cpp","c","hpp","frag","vert","txt"]

devops = ["retrieve.py"]
filenames = list(os.listdir(os.getcwd()))
for file in filenames:
    if file!=".git" and file not in devops:
        os.remove(file)

for file in os.listdir(project_source):
    full_path = os.path.normpath(os.path.join(project_source,file))
    if os.path.isfile(full_path):
        if os.path.splitext(full_path)[1].replace(".","") in extensions:
            copyfile(full_path,os.path.normpath(os.path.join(os.getcwd(),file)))

print("done")    