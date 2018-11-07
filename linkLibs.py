import argparse
import os

parser = argparse.ArgumentParser();
parser.add_argument('appDir', type=str, help='dir')

appDir = parser.parse_args().appDir + "/Contents/"

frameworksDir = appDir + "Frameworks/"

frameworkList = []


for filename in os.listdir(frameworksDir):
        framework = filename.split('.')[0]
        frameworkList.append(framework)

for frm in frameworkList:
    #print("@rpath/" + frm + "/Versions/A/" + frm)
    cmd = 'sudo install_name_tool -change '
    cmd += '@rpath/' + frm + '.framework/Versions/A/' + frm + ' '
    cmd += '@executable_path/../Frameworks/' + frm + '.framework/Versions/A/' + frm
    cmd += ' ' + appDir + 'MacOS/Memegeon'
    os.system(cmd)

os.chdir(appDir + "MacOS/")
print(os.system("otool -L Memegeon"))