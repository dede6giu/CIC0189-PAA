#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef unsigned long long ull;

string outBestPar(vector<vector<ull>> &s, ull i, ull j) {
    if (i > j) {
        return "";
    } else if (i == j) {
        string aux = "A" + to_string(i+1);
        return aux;
    } else {
        string aux = "( " + outBestPar(s, i, s[i][j]) + " " + outBestPar(s, s[i][j] + 1, j) + " )";
        return aux;
    }
}

int main() {
    // ios::sync_with_stdio(false); cin.tie(0); cout.tie(0);

    ull n;
    vector<ull> matrixSizes;
    const ull UPINF = -1;

    // Input
    cout << "\n";
    cout << "Especifique a quantidade de matrizes na multiplicacao:" << "\n";
    cin >> n;
    n += 1;
    
    cout << "\n";
    cout << "Especifique os valores dimensionais, em ordem! (" << n << ")" << "\n";
    for (ull i = 0; i < n; i++) {
        ull aux;
        cin >> aux;
        matrixSizes.push_back(aux);
    }

    // Processamento
    n -= 1;
    vector<vector<ull>> multp(n, vector<ull>(n, 0));
    vector<vector<ull>> indxs(n, vector<ull>(n, 0));

    for (ull l = 1; l < n; l++) {
        for (ull i = 0; i < n-l; i++) {
            ull j = i + l;
            multp[i][j] = UPINF;
            for (ull k = i; k < j; k++) {
                ull q = 0;
                q += multp[i][k] + multp[k+1][j];
                q += matrixSizes[i] * matrixSizes[k+1] * matrixSizes[j+1];
                if (q < multp[i][j]) {
                    multp[i][j] = q;
                    indxs[i][j] = k;
                }
            }
        }
    }


    // Output
    /*
    for (ull i = 0; i < n; i++) {
        for (ull j = 0; j < n; j++) {
            cout << multp[i][j] << " ";
        }
        cout << "\n";
    }

    for (ull i = 0; i < n; i++) {
        for (ull j = 0; j < n; j++) {
            cout << indxs[i][j] << " ";
        }
        cout << "\n";
    }
    */

    cout << "\n";
    cout << "====================";
    cout << "\n";
    cout << "\n";
    cout << "O numero minimo de multiplicacoes:" << "\n";
    cout << multp[0][n-1] << "\n";
    cout << "A melhor combinacao:" << "\n";
    cout << outBestPar(indxs, 0, n-1) << "\n";
    
    return 0;
}