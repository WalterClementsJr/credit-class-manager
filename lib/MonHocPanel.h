#ifndef MH_PANEL_H
#define MH_PANEL_H

#include <conio.h>

#include <iomanip>
#include <iostream>
#include <string>

#include "DsMonHoc.h"
#include "MonHoc.h"
#include "drawing.h"

using namespace std;

const int INSERT_X = DETAIL_X, INSERT_Y = DETAIL_Y + 1;

const string MH_FIELDS[] = {
    "Ma so: ", "Ten MH: ", "SLTC LT: ", "SLTC TH: "};

const unsigned int MH_FIELD_LIMITS[] = {10, 50, 2, 2};

void highlightIndex(MonHoc *list[], int index) {
    SetColor(GREY, BLACK);
    ShowCursor(false);

    gotoxy(TABLE_X, TABLE_Y + 2 + (index % MAX_TABLE_ROW) * 2);
    cout << setfill(' ') << left << setw(MH_FIELD_LIMITS[0]) << list[index]->ms
         << setw(MH_FIELD_LIMITS[1] + 2) << list[index]->ten
         << setw(MH_FIELD_LIMITS[2] + 6) << list[index]->sltclt
         << setw(MH_FIELD_LIMITS[2] + 6) << list[index]->sltcth;

    SetColor();
}

void dehighlightIndex(MonHoc *list[], int index) {
    SetColor(BLACK, WHITE);
    ShowCursor(false);

    gotoxy(TABLE_X, TABLE_Y + 2 + (index % MAX_TABLE_ROW) * 2);
    cout << setfill(' ') << left << setw(MH_FIELD_LIMITS[0]) << list[index]->ms
         << setw(MH_FIELD_LIMITS[1] + 2) << list[index]->ten
         << setw(MH_FIELD_LIMITS[2] + 6) << list[index]->sltclt
         << setw(MH_FIELD_LIMITS[2] + 6) << list[index]->sltcth;
    SetColor();
}

void clearInsertField(int index) {
    gotoxy(INSERT_X + MH_FIELDS[index].length(), INSERT_Y + index * 2);
    cout << string(MH_FIELD_LIMITS[index], ' ');
}

void printInsertMHField(int index, string input) {
    ShowCur(false);

    clearInsertField(index);
    gotoxy(INSERT_X + MH_FIELDS[index].length(), INSERT_Y + index * 2);
    cout << input;

    ShowCur(true);
}

string insertMonHoc(DsMonHoc &dsmh, MonHoc *list[]) {
    string ms = "";
    gotoxy(INSERT_X, INSERT_Y - 1);
    cout << "THEM MON HOC";

    string input[] = {"", "", "0", "0"};

    // print fields
    for (unsigned i = 0; i < sizeof(MH_FIELDS) / sizeof(MH_FIELDS[0]); i++) {
        gotoxy(INSERT_X, INSERT_Y + i * 2);
        cout << MH_FIELDS[i];
        printInsertMHField(i, input[i]);
    }

    gotoxy(INSERT_X + MH_FIELDS[0].length(), INSERT_Y);

    unsigned index = 0;
    unsigned count = 0;
    unsigned key;
    unsigned exit = 0;

    bool pass = true;

    while (exit == 0) {
        pass = true;
        key = _getch();

        // catch special input first
        if (key == 224 || key == 0) {
            key = _getch();
            if (key == KEY_UP) {
                index = index <= 0 ? 3 : index - 1;
                count = !input[index].empty() ? input[index].length() : 0;
                gotoxy(INSERT_X + MH_FIELDS[index].length() + count,
                       INSERT_Y + index * 2);
                printInsertMHField(index, input[index]);
            } else if (key == KEY_DOWN) {
                index = index >= 3 ? 0 : index + 1;
                count = !input[index].empty() ? input[index].length() : 0;
                gotoxy(INSERT_X + MH_FIELDS[index].length() + count,
                       INSERT_Y + index * 2);
                printInsertMHField(index, input[index]);
            } else if (key == KEY_LEFT) {
                count = count <= 0 ? input[index].length() : (count - 1);
                gotoxy(INSERT_X + MH_FIELDS[index].length() + count,
                       INSERT_Y + index * 2);
            } else if (key == KEY_RIGHT) {
                count = count >= input[index].length() ? 0 : (count + 1);
                gotoxy(INSERT_X + MH_FIELDS[index].length() + count,
                       INSERT_Y + index * 2);
            }
        } else if (key == BACKSPACE) {
            // if input is empty
            if (input[index].empty() || count == 0) {
                continue;
            }

            input[index].erase(count - 1, 1);
            count--;
            printInsertMHField(index, input[index]);

            // move cursor
            gotoxy(INSERT_X + MH_FIELDS[index].length() + count,
                   INSERT_Y + index * 2);
        } else if (key == ESC) {
            clearDetail();
            break;
        } else if (key == ENTER) {
            if (index == 3) {
                // check if one of the inputs is empty
                for (string s : input) {
                    if (s.empty()) {
                        pass = false;
                        break;
                    }
                }

                // check format

                if (!pass) {
                    displayNotification("Hay dien day du thong tin.", RED);
                    continue;
                } else {
                    // check if ms already exist
                    if (dsmh.search(input[0]) != NULL) {
                        displayNotification("MS mon hoc da ton tai.", RED);

                        cout << input[0] << endl
                             << input[1] << endl
                             << input[2] << endl
                             << input[3] << endl;
                        continue;
                    }

                    // confirm insert
                    clearNotification();
                    SetColor(BLACK, RED);
                    gotoxy(INSERT_X, INSERT_Y + 10);
                    cout << "Xac nhan them? Y/N";
                    SetColor();
                    key = _getch();

                    while (key != ESC) {
                        if (key == 0 || key == 224) _getch();
                        // yes
                        else if (key == 'y' || key == 'Y') {
                            gotoxy(INSERT_X, INSERT_Y + 10);
                            cout << string(40, ' ');

                            // insert
                            dsmh.insert(input[0], input[1], stoi(input[2]), stoi(input[3]));
                            dsmh.write();
                            exit = 1;
                            ms = input[0];
                            break;
                        }
                        // no
                        else if (key == 'n' || key == 'N') {
                            gotoxy(INSERT_X, INSERT_Y + 10);
                            cout << string(40, ' ');
                            break;
                        } else {
                            key = _getch();
                        }
                    }
                }
                // handle submit
                cout << input[0] << ","
                     << input[1] << ","
                     << input[2] << ","
                     << input[3];
            } else {
                index++;
                // move to end of next row if input is not empty
                count = !input[index].empty() ? input[index].length() : 0;
                gotoxy(INSERT_X + MH_FIELDS[index].length() + count,
                       INSERT_Y + index * 2);
            }
        } else {
            // catch character input
            printInsertMHField(index, input[index]);

            if (index == 0) {
                // if field is ms
                if ((key >= 'a' && key <= 'z') || (key >= 'A' && key <= 'Z')) {
                    // out of range
                    if (input[index].length() >= MH_FIELD_LIMITS[index]) {
                        continue;
                    }
                    input[index].insert(count, 1, char(key));
                    count++;
                    printInsertMHField(index, input[index]);
                    gotoxy(INSERT_X + MH_FIELDS[index].length() + count,
                           INSERT_Y + index * 2);
                }
            } else if (index == 1) {
                // field is ten
                if ((key >= 'a' && key <= 'z') || (key >= 'A' && key <= 'Z')) {
                    if (input[index].length() >= MH_FIELD_LIMITS[index]) {
                        continue;
                    }
                    input[index].insert(count, 1, char(key));
                    count++;
                    printInsertMHField(index, input[index]);
                    gotoxy(INSERT_X + MH_FIELDS[index].length() + count,
                           INSERT_Y + index * 2);
                }

            } else if (index == 2) {
                // field is sltclt
                if (key >= '0' && key <= '9') {
                    if (input[index].length() >= MH_FIELD_LIMITS[index]) {
                        continue;
                    }
                    input[index].insert(count, 1, char(key));
                    count++;
                    printInsertMHField(index, input[index]);
                    gotoxy(INSERT_X + MH_FIELDS[index].length() + count,
                           INSERT_Y + index * 2);
                }
            } else if (index == 3) {
                // field is sltcth
                if (key >= '0' && key <= '9') {
                    if (input[index].length() >= MH_FIELD_LIMITS[index]) {
                        continue;
                    }
                    input[index].insert(count, 1, char(key));
                    count++;
                    printInsertMHField(index, input[index]);
                    gotoxy(INSERT_X + MH_FIELDS[index].length() + count,
                           INSERT_Y + index * 2);
                }
            }
        }
    }
    return input[0];
}

string editMonHoc(DsMonHoc &dsmh, MonHoc *list[], int index) {
    // TODO
    return "";
}

void deleteMonHoc(DsMonHoc &dsmh, MonHoc *list[], int index) {
    // xác nhận xóa
    displayNotification("Xac nhan xoa " + list[index]->ten + "? Y/N");
    int key = _getch();

    while (true) {
        if (key == 0 || key == 224) {
            _getch();
        } else if (key == 'y' || key == 'Y') {
            // TODO delete from ds and temp
            break;
        } else if (key == 'n' || key == 'N') {
            break;
        } else {
            key = _getch();
        }
    }
}

// display MonHoc to table based on index
void loadMonHocToTable(MonHoc *list[], int length, int index) {
    ShowCur(false);

    int x = TABLE_X, y = TABLE_Y + 2;
    int currentPage = index / MAX_TABLE_ROW;

    int rowsLeft = length - currentPage * MAX_TABLE_ROW;
    rowsLeft = rowsLeft > MAX_TABLE_ROW ? MAX_TABLE_ROW : rowsLeft;

    for (int i = 0; i < rowsLeft; i++) {
        gotoxy(x, y);
        cout << setfill(' ') << left << setw(MH_FIELD_LIMITS[0])
             << list[index + i]->ms << setw(MH_FIELD_LIMITS[1] + 2)
             << list[index + i]->ten << setw(MH_FIELD_LIMITS[2] + 6)
             << list[index + i]->sltclt << setw(MH_FIELD_LIMITS[2] + 6)
             << list[index + i]->sltcth;
        drawRow(x, y + 1, TABLE_WIDTH);
        y += 2;
    }
}

int initMHPanel(DsMonHoc &dsmh) {
    SetColor(BLACK, WHITE);

    gotoxy(TABLE_X, TABLE_Y);
    cout << "Ma so";
    gotoxy(TABLE_X + MH_FIELD_LIMITS[0], TABLE_Y);
    cout << "Ten mon hoc";
    gotoxy(TABLE_X + 62, TABLE_Y);
    cout << "STC LT";
    gotoxy(TABLE_X + 70, TABLE_Y);
    cout << "STC TH";

    drawRow(TABLE_X, TABLE_Y + 1, TABLE_WIDTH);

    int key, exit = 0,
             // số hàng còn lại
        nOfRowRemains,
             // index trong list
        index = 0,
             // current page
        currentPage,
             // max number of pages
        nPage;

    MonHoc *list[10000];
    // load to array
    dsmh.toArray(list);
    int dsLength = dsmh.getCount();

    // highlight if not empty
    if (dsLength > 0) {
        loadMonHocToTable(list, dsLength, index);
        highlightIndex(list, index);
    }

    while (exit == 0) {
        currentPage = index / MAX_TABLE_ROW;
        nPage = dsmh.getCount() / MAX_TABLE_ROW;
        nOfRowRemains = dsLength - currentPage * MAX_TABLE_ROW;
        nOfRowRemains =
            nOfRowRemains > MAX_TABLE_ROW ? MAX_TABLE_ROW : nOfRowRemains;

        key = _getch();
        // special keys
        if (key == 0 || key == 224) {
            // "0" keys
            if (key == 0) {
                key = _getch();
                // change tab keys
                if (key == KEY_F1 || key == KEY_F2 || key == KEY_F4 ||
                    key == KEY_F5) {
                    return key - 59;
                }
            }
            // "224" keys
            else if (key == 224) {
                key = _getch();
                if (key == KEY_UP) {
                    // first row -> last row
                    if (index <= currentPage * MAX_TABLE_ROW) {
                        dehighlightIndex(list, index);
                        index = currentPage * MAX_TABLE_ROW + nOfRowRemains - 1;
                        highlightIndex(list, index);
                    } else {
                        index--;
                        highlightIndex(list, index);
                        dehighlightIndex(list, index + 1);
                    }
                } else if (key == KEY_DOWN) {
                    // last row -> first row
                    if (index >=
                        currentPage * MAX_TABLE_ROW + nOfRowRemains - 1) {
                        dehighlightIndex(list, index);
                        index = currentPage * MAX_TABLE_ROW;
                        highlightIndex(list, index);
                    } else {
                        index++;
                        highlightIndex(list, index);
                        dehighlightIndex(list, index - 1);
                    }
                } else if (key == KEY_LEFT) {
                    index =
                        (currentPage > 0 ? currentPage - 1 : 0) * MAX_TABLE_ROW;
                    clearTable();
                    loadMonHocToTable(list, dsLength, index);
                    highlightIndex(list, index);
                } else if (key == KEY_RIGHT) {
                    currentPage =
                        currentPage >= nPage ? nPage : currentPage + 1;
                    index = currentPage * MAX_TABLE_ROW;
                    clearTable();
                    loadMonHocToTable(list, dsLength, index);
                    highlightIndex(list, index);
                } else if (key == INSERT) {
                    // insert
                    insertMonHoc(dsmh, list);
                    index = 0;
                    loadMonHocToTable(list, dsLength, index);
                    highlightIndex(list, index);
                } else if (key == DEL) {
                    // remove
                    deleteMonHoc(dsmh, list, index);
                    index = 0;
                    loadMonHocToTable(list, dsLength, index);
                    highlightIndex(list, index);
                }
            }
        } else if (key == TAB) {
            // search + edit
            // searchMonHoc(dsmh);
            loadMonHocToTable(list, dsLength, index);
            highlightIndex(list, index);
        } else if (key == ENTER) {
            // edit row
            editMonHoc(dsmh, list, index);
            index = 0;
            // loadMonHocToTable(list, index);
            highlightIndex(list, index);
        }
    }

    SetColor();
    return 0;
}

#endif
