#include <iostream>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <vector>

using namespace std;

clock_t start_time, end_time;

// 随机生成矩阵链的维度
void generateRandomDimensions(vector<int>& dims, int n) {
    dims.push_back(rand() % 98 + 3); // 第一个矩阵的行数
    for (int i = 1; i < n; i++) {
        dims.push_back(rand() % 98 + 3); // 随机生成后续矩阵的列数
    }
    dims.push_back(rand() % 98 + 3); // 最后一个矩阵的列数
}

// 备忘录法
int memoizationMatrixChainMultiplication(vector<vector<int>>& dp, const vector<int>& dims, int i, int j) {
    if (i == j) return 0;
    if (dp[i][j] != -1) return dp[i][j];
    int minCost = INT_MAX;
    for (int k = i; k < j; k++) {
        int cost = memoizationMatrixChainMultiplication(dp, dims, i, k) + memoizationMatrixChainMultiplication(dp, dims, k + 1, j) + dims[i - 1] * dims[k] * dims[j];
        minCost = min(minCost, cost);
    }
    dp[i][j] = minCost;
    return dp[i][j];
}

// 动态规划法
int dynamicProgrammingMatrixChainMultiplication(const vector<int>& dims, int n) {
    vector<vector<int>> dp(n, vector<int>(n, 0));
    for (int length = 2; length < n; length++) {
        for (int i = 0; i < n - length; i++) {
            int j = i + length;
            dp[i][j] = INT_MAX;
            for (int k = i; k < j; k++) {
                int q = dp[i][k] + dp[k + 1][j] + dims[i - 1] * dims[k] * dims[j];
                dp[i][j] = min(dp[i][j], q);
            }
        }
    }
    return dp[0][n - 1];
}

// 将运行结果写入CSV文件
void writeResultsToCSV(string filename, int test_size, double timesMemoization, double timesDP) {
    ofstream outFile(filename, ios::app); // 追加模式写入文件
    if (outFile.is_open()) {
        outFile << test_size << "," 
                << fixed << setprecision(4) << timesMemoization << ","
                << fixed << setprecision(4) << timesDP << endl;
        outFile.close();
    } else {
        cout << "无法打开文件写入结果!" << endl;
    }
}

int main() {
    srand(time(NULL)); // 随机数种子

    // 定义测试数据量（矩阵链的长度）
    vector<int> test_sizes = {200, 500, 800, 1000};  // 测试数据规模

    // 创建并初始化CSV文件，写入表头
    ofstream outFile("matrix_chain_results1.csv");
    if (outFile.is_open()) {
        outFile << "Matrix Chain Length,Memoization Time,Dynamic Programming Time\n"; // CSV 表头
        outFile.close();
    } else {
        cout << "无法创建CSV文件!" << endl;
        return 1;
    }

    // 遍历每个测试数据量
    for (int size : test_sizes) {
        vector<int> dims;
        generateRandomDimensions(dims, size);  // 随机生成矩阵链的维度

        cout << "正在测试矩阵链长度: " << size << endl;

        // 记录不同方法的运行时间
        double timesMemoization = 0.0, timesDP = 0.0;

        // 测试备忘录法
        vector<vector<int>> dp(size + 1, vector<int>(size + 1, -1));
        start_time = clock();
        memoizationMatrixChainMultiplication(dp, dims, 1, size - 1);
        end_time = clock();
        timesMemoization = double(end_time - start_time) / CLOCKS_PER_SEC;

        // 测试动态规划法
        start_time = clock();
        dynamicProgrammingMatrixChainMultiplication(dims, size);
        end_time = clock();
        timesDP = double(end_time - start_time) / CLOCKS_PER_SEC;

        // 将测试结果写入CSV文件
        writeResultsToCSV("matrix_chain_results1.csv", size, timesMemoization, timesDP);
        cout << "测试完成，结果已保存至 'matrix_chain_results1.csv'" << endl;
    }

    cout << "所有测试完成，结果已保存至 'matrix_chain_results1.csv' 文件中。" << endl;

    return 0;
}
