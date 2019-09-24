import os
from shutil import copyfile

project_source = os.path.normpath(os.path.abspath("../raytraceproject/raytraceproject"))

print("working...")
#single level project for now
extensions = ["h","cpp","c","hpp","frag","vert","txt"]
ignores=["Dependencies","Debug"]
ignores = list(map(lambda f:os.path.normpath(os.path.join(project_source,f)),ignores))
devops = ["retrieve.py"]
filenames = list(os.listdir(os.getcwd()))
for file in filenames:
    if file!=".git" and file not in devops:
        os.remove(file)
folder_stack =[""]
while len(folder_stack) >0 :
    search_folder = folder_stack.pop()
    for file in os.listdir(os.path.normpath(os.path.join(project_source,search_folder))):
        full_path = os.path.normpath(os.path.join(project_source,search_folder,file))
        if os.path.isfile(full_path):
            if os.path.splitext(full_path)[1].replace(".","") in extensions:
                copyfile(full_path,os.path.normpath(os.path.join(os.getcwd(),file)))
        if os.path.isdir(full_path):
            if full_path not in ignores:
                folder_stack.append(os.path.normpath(os.path.join(search_folder,file)))
print("done")    