/*
========= AGENDAMENTO DE SALAS =========

O código a seguir aceita entradas do
formato (id, start, end) onde id é uma
string, start e end são horários no
formato "HH:MM".

Em seguida, é usado um processo guloso
no qual se assume que a solução correta
pode ser obtida pela escolha óptima em
todo turno.

Aqui, a escolha óptima é sempre colocar
a próxima reunião (ordenada de acordo
com o horário de ínicio) em qualquer
sala que esteja disponível. Independente
das escolhas, sempre será óptimo colocar
uma reunião em qualquer sala no momento
em que tal inicia: se outra vier a 
começar durante esse período, não há um
arranjo que reduza a quantidade mínima
necessária de salas (duas).

Essa solução é particularmente
ineficiente em fornecer a quantidade de
salas utilizadas, mas ela fornece ambas
a quantidade e a distribuição das 
reuniões nas salas. Uma solução sem a
distribuição é possível usando delta
arrays, e o resultado é mais eficiente.
*/

#include <bits/stdc++.h>
using namespace std;

typedef unsigned int ui;
typedef pair<ui, ui> puu;
typedef tuple<string, puu, puu> tspp;

ui compTimeInc(const puu &a, const puu &b) {
    // Retorna a ordem dos horários
    // 0 - a > b, deve ocorrer troca
    // 1 - a < b, ordem correta
    // 2 - a == b

    if (a.first > b.first) {
        return 0;
    } else if (a.first == b.first) {
        if (a.second > b.second) {
            return 0;
        } else if (a.second == b.second) {
            return 2;
        }
        return 1;
    }
    return 1;
}

bool compSpecial(const tspp &a, const tspp &b) {
    // Ordenação especial. Prioridade:
    // 1. Início da reunião (mais cedo primeiro)
    // 2. Término da reunião (mais cedo primeiro)
    // 3. ID (ordem alfabética) - desempate

    switch (compTimeInc(get<1>(a), get<1>(b))) {
        case 2:
            switch (compTimeInc(get<2>(a), get<2>(b))) {
                case 2:
                    if (get<0>(a) > get<0>(b)) {
                        return false;
                    }
                    return true;
                case 1:
                    return true;
                case 0:
                default:
                    return false;
            }
        case 1:
            return true;
        case 0:
        default:
            return false;
    }
}

puu hoursToInt(const string &timeStamp) {
    string shour = "", sminute = "";
    int hour = 0, minute = 0;
    if (timeStamp.length() != 5 || timeStamp[2] != ':') {
        return pair(60, 60);
    }
    shour += timeStamp[0];
    shour += timeStamp[1];
    sminute += timeStamp[3];
    sminute += timeStamp[4];
    try {
        hour = stoi(shour);
        minute = stoi(sminute);
        if (hour >= 24 || minute >= 60) {
            return pair(60, 60);
        }
        return pair(hour, minute);
    } catch(exception &e) {
        return pair(60, 60);
    }
}

vector<string> split(const string &str, const char &delim) {
    string::const_iterator iter = str.begin();
    vector<string> result;
    while(iter != str.end()) {
        while (*iter == delim) {
            iter++;
        }
        string::const_iterator temp = find(iter, str.end(), delim);
        if(iter != str.end())
            result.push_back(string(iter, temp));
        iter = temp;
    }
    return result;
}

int main() {
    
    vector<tspp> dataRaw;
    unordered_set<string> IDs;


    // Entrada

    cout << "\n";
    cout << "Forneca as reunioes. Fornecer \"0 0 0\" termina a entrada." << "\n";
    cout << "(id Any, start HH:MM, end HH:MM)" << "\n";
    while (true) {
        string auxInp;
        getline(cin, auxInp);
        vector<string> auxVec = split(auxInp, ' ');
        if (auxVec.size() != 3) {
            cout << "Quantidade incorreta de parametros." << "\n";
            continue;
        }
        
        if (auxVec[0] == "0" && auxVec[1] == "0" && auxVec[2] == "0") {
            break;
        }
        
        puu start = hoursToInt(auxVec[1]);
        puu end = hoursToInt(auxVec[2]);
        if (start.first == 60 || end.first == 60) {
            cout << "Horario mal-formatado." << "\n";
            continue;
        }
        ui auxComp = compTimeInc(start, end);
        if (auxComp == 2) {
            cout << "Horario de inicio e de termino devem ser distintos." << "\n";
            continue;
        } else if (auxComp == 0) {
            cout << "Horario de inicio deve ocorrer antes do termino." << "\n";
            continue;
        }

        if (IDs.count(auxVec[0])) {
            cout << "ID ja foi registrado." << "\n";
            continue;
        } else {
            IDs.insert(auxVec[0]);
        }

        dataRaw.push_back(tuple(auxVec[0], start, end));
    }


    // Processamento
    
    vector<vector<string>> rooms;
    vector<puu> latest;
    sort(dataRaw.begin(), dataRaw.end(), compSpecial);
    
    if (!dataRaw.empty()) {
        vector<string> newRoom;
        newRoom.push_back(get<0>(dataRaw[0]));
        rooms.push_back(newRoom);
        latest.push_back(get<2>(dataRaw[0]));
    }
    for (ui i = 1; i < dataRaw.size(); i++) {
        tspp curr = dataRaw[i];
        bool foundRoom = false;

        for (ui j = 0; j < latest.size(); j++) {
            if (foundRoom) break;
            switch (compTimeInc(latest[j], get<1>(curr))) {
                case 2:
                case 1:
                    // Tem uma sala disponível
                    rooms[j].push_back(get<0>(curr));
                    latest[j] = get<2>(curr);
                    foundRoom = true;
                case 0:
                default:
                    continue;
            }
        }

        if (!foundRoom) {
            // Nova sala necessária
            vector<string> newRoom;
            newRoom.push_back(get<0>(dataRaw[i]));
            rooms.push_back(newRoom);
            latest.push_back(get<2>(dataRaw[i]));
        }
    }
    

    // Saída

    cout << "\n";
    if (dataRaw.empty()) {
        cout << "Nao foram fornecidas reunioes.";
    } else {
        cout << "Lista de reunioes para cada sala:" << "\n";
        for (ui i = 0; i < rooms.size(); i++) {
            cout << "- Sala " << i+1 << ": ";
            ui amount = rooms[i].size();
            for (ui j = 0; j < amount; j++) {
                cout << rooms[i][j];
                if (j < amount-1) {
                    cout << ", ";
                } else {
                    cout << ".\n";
                }
            }
        }
    }
    return 0;
}