#include <iostream>
#include <ctime>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <vector>
#include <string>
using namespace std;
clock_t start_time, end_time;

// 随机生成字符串 X 和 Y
void generateRandomString(string& str, int length) {
    for (int i = 0; i < length; i++) {
        str += (rand() % 26) + 'A';  // 随机生成一个大写字母
    }
}

// 动态规划法解决最长公共子序列问题
int dynamicProgrammingLCS(const string& X, const string& Y, int m, int n) {
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (X[i - 1] == Y[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    return dp[m][n];
}

// 将运行结果写入CSV文件
void writeResultsToCSV(const string& filename, int test_size, double timesDP) {
    ofstream outFile(filename, ios::app); // 追加模式写入文件
    if (outFile.is_open()) {
        outFile << test_size << ","
                << fixed << setprecision(4) << timesDP << endl;
        outFile.close();
    } else {
        cout << "无法打开文件写入结果!" << endl;
    }
}

int main() {
    srand(time(NULL)); // 随机数种子
    // 定义测试数据量
    vector<int> test_sizes = {1000, 2000, 5000, 10000, 20000};  // 测试数据规模
    // 创建并初始化CSV文件，写入表头
    ofstream outFile("lcs_results.csv");
    if (outFile.is_open()) {
        outFile << "String Length,Dynamic Programming Time\n"; // CSV 表头
        outFile.close();
    } else {
        cout << "无法创建CSV文件!" << endl;
        return 1;
    }

    // 遍历每个测试数据量
    for (int size : test_sizes) {
        string X, Y;
        generateRandomString(X, size);  // 随机生成字符串 X
        Y = X;  // 设置 Y 与 X 相同，方便比较
        cout << "正在测试字符串长度: " << size << endl;
        // 记录动态规划法的运行时间
        double timesDP = 0.0;
        // 测试动态规划法
        start_time = clock();
        dynamicProgrammingLCS(X, Y, size, size);
        end_time = clock();
        timesDP = double(end_time - start_time) / CLOCKS_PER_SEC;
        // 将测试结果写入CSV文件
        writeResultsToCSV("lcs_results.csv", size, timesDP);
        cout << "测试完成，结果已保存至 'lcs_results.csv'" << endl;
    }
    cout << "所有测试完成，结果已保存至 'lcs_results.csv' 文件中。" << endl;
    return 0;
}
