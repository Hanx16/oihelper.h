#include "helper.hpp"
#include <filesystem>
#define byte Byte
#include <windows.h>
#undef byte

using filesystem::directory_iterator, filesystem::path, filesystem::remove;

mutex g_mutexWait;
condition_variable g_condWait;

const int MaxTime = 5000;

path TexeFile, Tinput;
int returnValue;
bool enableSPJ;

void RunExeThread() {
    string cmd = "\"\"" + TexeFile.filename().string() + "\" < \"" + Tinput.filename().string() + "\" > tempOutput\"";
    returnValue = system(cmd.c_str());
    g_condWait.notify_one();
}

vector<string> GetNumber(string str) {
    vector<string> res;
    string curNumber;
    for (char c : str) {
        if (isdigit(c))
            curNumber += c;
        else {
            res.push_back(curNumber);
            curNumber = "";
        }
    }
    return res;
}

short CompareNumber(string Num1, string Num2) {
    if (Num1.length() != Num2.length())
        return Num1.length() < Num2.length();
    for (int i = 0; i < (int)Num1.length(); ++i)
        if (Num1[i] != Num2[i])
            return Num1[i] < Num2[i];
    return -1;
}

void ColorPrint(string str, int color) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | color);
    cout << str;
    SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | 7);
}

signed main() {
    vector<path> file;
    for (auto &i : directory_iterator(".\\"))
        file.push_back(i.path());
    vector<path> exe, inFile;
    map<path, bool> ansFile, outFile;
    for (auto &i : file) {
        if (i.extension().string() == ".exe")
            exe.push_back(i);
        else if (i.extension().string() == ".in")
            inFile.push_back(i);
        else if (i.extension().string() == ".out")
            outFile[i.stem().string()] = true;
        else if (i.extension().string() == ".ans")
            ansFile[i.stem().string()] = true;
    }
    path exeFile;
    ColorPrint("", 7);
    if (exe.size() == 0) {
        ColorPrint("There are no [.exe] files in the directory.\n", 4);
        return 0;
    } else if (exe.size() == 1)
        exeFile = exe[0];
    else {
        int id = 0;
        for (auto &i : exe)
            cout << (id++) << ". [" << i.filename().string() << "]\n";
        cout << "Choose your program: ";
        int choice;
        cin >> choice;
        int cnt = 0;
        while (choice < 0 || choice >= (int)exe.size()) {
            ++cnt;
            if (cnt >= 3) {
                cout << "Too many wrong tries!\n";
                return 0;
            }
            cout << "Wrong choice, plz input a right one: ";
            cin >> choice;
        }
        exeFile = exe[choice];
    }
    ColorPrint("Your program is [" + exeFile.filename().string() + "].\n", 11);
    enableSPJ = false;
    for (auto &i : exe)
        if (i.filename().string() == "spj.exe")
            enableSPJ = true;
    if (enableSPJ)
        ColorPrint("Enable special judge.\n", 6);
    auto cmp = [](const path &fileA, const path &fileB)->bool {
        vector<string> List1 = GetNumber(fileA.filename().string());
        vector<string> List2 = GetNumber(fileB.filename().string());
        for (int i = 0; i < (int)min(List1.size(), List2.size()); ++i) {
            short result = CompareNumber(List1[i], List2[i]);
            if (result != -1)
                return result;
        }
        return List1.size() < List2.size();
    };
    sort(inFile.begin(), inFile.end(), cmp);
    for (auto input : inFile) {
        string answer = "";
        if (ansFile.count(input.stem().string()))
            answer = input.stem().string() + ".ans";
        else if (outFile.count(input.stem().string()))
            answer = input.stem().string() + ".out";
        if (answer == "") continue;
        cout << "--------------------------------------------------------------\n";
        cout << "For testcase [" << input.filename().string() << " / "
                                 << answer << "]:\n";
        Tinput = input, TexeFile = exeFile;
        thread threadRun(RunExeThread);
        double ST = clock();
        unique_lock<mutex> lockWait(g_mutexWait);
        auto cvsts = g_condWait.wait_for(lockWait, chrono::milliseconds(MaxTime));
        if (cvsts == cv_status::timeout) {
            ColorPrint("Time Limit Exceed!\n", 14);
        } else {
            cout << "Time cost: " << (clock() - ST) / CLOCKS_PER_SEC * 1000 << "ms\n";
            if (returnValue != 0)
                ColorPrint("Runtime Error!\n", 13);
            else {
                if (!enableSPJ) {
                    if (diffFile(answer, "tempOutput"))
                        ColorPrint("Wrong Anwer!\n", 4);
                    else
                        ColorPrint("Accepted!\n", 2);
                } else {
                    system(("\"spj.exe " + input.filename().string() + " tempOutput " +
                            answer + "\"").c_str());
                }
            }
            cout << "Exit with code " << returnValue << ".\n";
        }
        threadRun.detach();
        pthread_cancel(threadRun.native_handle());
        if (cvsts == cv_status::timeout) {
            system(("taskkill /f /im " + exeFile.filename().string()).c_str());
        }
        system("del tempOutput");
    }
}