#include <bits/stdc++.h>

using namespace std;

/**
 * 参数列表
 */
namespace Args {
    bool chkSpace = false,              // 是否忽略间隔符，-S
         stopOnFirst = false,           // 在第一个不同的位置停止比较，-T
         showHelp = false,              // 显示帮助，-H
         showInfo = true;               // 显示返回信息，-I
    vector<string> fileList;            // 文件列表
}

void getHelp() {
    const string help(
        "Help:\n"
        "diff <-S> <-T> <-H> <-I> [filename1] [filename2]\n"
        "\n"
        "-S: Ignore spaces;\n"
        "-T: Stop on the first different place;\n"
        "-I: Don't show return infomation;\n"
        "-H: Show helps.\n"
    );
    cout << help;
}

/**
 * 解析命令行中的参数。
 * 返回是否解析成功。
 */
bool parseArgs(vector<string> argList) {
    for (auto arg: argList) {
        if ((arg[0] == '-' || arg[0] == '/') && arg.length() == 2u) {
            if (arg[1] >= 'a' && arg[1] <= 'z') { // 小写转大写
                arg[1] += 'A' - 'a';
            }

            switch (arg[1]) { // 判断命令种类
            case 'S':
                Args::chkSpace = true;
                break;

            case 'T':
                Args::stopOnFirst = true;
                break;

            case 'H':
                Args::showHelp = true;
                break;

            case 'I':
                Args::showInfo = false;
                break;

            default:
                return false;
            }
        } else {
            Args::fileList.emplace_back(arg);
        }
    }

    if (Args::fileList.size() != 2u && !Args::showHelp) { // 文件数量不为 2
        return false;
    }

    return true;
}

/**
 * 根据参数输出信息。
 */
void outputInfo(int pos, string str1, string str2) {
    if (!Args::showInfo) { // 不显示信息
        return ;
    }

    const size_t lenLimit = 50;
    const size_t fileLen = max(Args::fileList[0].length(), Args::fileList[1].length());
    if (str1.length() < lenLimit && str2.length() < lenLimit) { // 不同串长度较小
        cout << pos << ": --------------------\n";
        cout << setw(fileLen) << Args::fileList[0] << setw(0) << ": " << str1 << '\n';
        cout << setw(fileLen) << Args::fileList[1] << setw(0) << ": " << str2 << '\n';
    } else {
        size_t diffPlace = 0;
        for (; diffPlace < str1.length() && diffPlace < str2.length()
                && str1[diffPlace] == str2[diffPlace]; ++diffPlace); // 找到第一个不同的位置
        cout << pos << ", " << diffPlace << ": --------------------\n";
        size_t showL = max((size_t)0, diffPlace - lenLimit / 2); // 显示的最左段
        size_t showR = min(diffPlace + lenLimit / 2, min(str1.length(), str2.length())); // 显示的最右段
        cout << setw(fileLen) << Args::fileList[0] << setw(0) << ": ..." << str1.substr(showL, showR - showL + 1) << "...\n";
        cout << setw(fileLen) << Args::fileList[1] << setw(0) << ": ..." << str2.substr(showL, showR - showL + 1) << "...\n";
        cout << setw(fileLen) << " " << setw(0) << "     ";
        for (size_t i = showL; i < diffPlace; ++i) {
            cout << " ";
        }

        cout << "^\n";
    }
}

/** 
 * 处理命令。
 * 返回值：
 * -1: 文件异常
 * 0:  无区别
 * 1:  有区别
 * 2:  仅显示帮助文档
 */
short processArgs() {
    if (Args::showHelp) { // 显示帮助
        getHelp();
        return 2;
    }

    ifstream file1(Args::fileList[0]);
    ifstream file2(Args::fileList[1]);
    if (!file1.is_open() || !file2.is_open()) { // 文件异常
        return -1;
    }

    bool isDiff = false;
    if (!Args::chkSpace) { // 逐字符比较
        char buf1, buf2;
        int i = 0; // 当前比较的位置
        while (buf1 = file1.get(), buf2 = file2.get(), (buf1 != EOF && buf2 != EOF)) {
            if (buf1 == buf2) {
                ++i;
                continue;
            }

            isDiff = true;
            string strBuf1, strBuf2;
            strBuf1 += buf1, strBuf2 += buf2;
            outputInfo(i, strBuf1, strBuf2);
            if (Args::stopOnFirst) {
                break;
            }

            ++i;
        }

        if ((buf1 == EOF) ^ (buf2 == EOF)) { // 文件长度不同
            isDiff = true;
            if (Args::showInfo) {
                cout << "Diffrent length.\n";
        }
    }
    } else { // 忽略间隔符比较
        string buf1, buf2;
        int i = 0; // 当前比较的位置
        while (true) {
            auto &res1 = file1 >> buf1, &res2 = file2 >> buf2;
            if (!res1 || !res2) {
                if ((!res1 && res2) || (res1 && !res2)) { // 判断文件长度
                    isDiff = true;
                    if (Args::showInfo) {
                        cout << "Diffrent length.\n";
                    }
                }

                break;
            }
            if (buf1 == buf2) {
                ++i;
                continue;
            }

            isDiff = true;
            outputInfo(i, buf1, buf2);
            if (Args::stopOnFirst) {
                break;
            }

            ++i;
        }
    }

    if (!isDiff) { // 无不同
        if (Args::showInfo) {
            cout << "No differences.\n";
        }
        
        return 0;
    } else {
        return 1;
    }
}

signed main(int argc, char *argv[]) {
    vector<string> argList;
    for (int i = 1; i < argc; ++i) {
        argList.emplace_back(argv[i]);
    }
    
    bool noParseError = parseArgs(argList);
    if (!noParseError) {
        cout << "Command parse error!\n";
        return -2;
    }

    short resp = processArgs();
    if (resp == -1) {
        cout << "Can't open this file!\n";
    }

    return resp;
}