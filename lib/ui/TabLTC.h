#ifndef LTC_PANEL_H
#define LTC_PANEL_H

#include <conio.h>
#include <windows.h>

#include <iomanip>
#include <iostream>
#include <regex>
#include <string>

#include "model/DangKy.h"
#include "model/DsMonHoc.h"
#include "model/LTC.h"
#include "model/MonHoc.h"
#include "model/SinhVien.h"
#include "ui/TabDangKy.h"
#include "ui/drawing.h"
#include "util/import.h"

using std::cout;
using std::endl;
using std::string;
using std::setfill;
using std::setw;
using std::left;

const string LTC_FIELDS[] = {
    "Ma MH: ", "Nien khoa: ", "Hoc ky: ", "Nhom: ",
    "So sv min: ", "So sv max: ", "Huy (0, 1): "};

const unsigned int LTC_LIMITS[] = {10, 9, 1, 1, 3, 3, 1};

void highlightIndex(LTC *list[], int index) {
    SetColor(BLACK, BLUE);
    ShowCursor(false);

    gotoxy(TABLE_X, TABLE_Y + 2 + (index % MAX_TABLE_ROW) * 2);
    cout << setfill(' ') << left << setw(15) << list[index]->maLTC << setw(15)
         << list[index]->maMH << setw(15) << list[index]->nienKhoa << setw(10)
         << list[index]->hocKy << setw(8) << list[index]->nhom << setw(10)
         << list[index]->min << setw(10) << list[index]->max << setw(10)
         << list[index]->huy << setw(10) << list[index]->dsdk->getSoSVDK();

    SetColor();
}

void dehighlightIndex(LTC *list[], int index) {
    SetColor();
    ShowCursor(false);

    gotoxy(TABLE_X, TABLE_Y + 2 + (index % MAX_TABLE_ROW) * 2);
    cout << setfill(' ') << left << setw(15) << list[index]->maLTC << setw(15)
         << list[index]->maMH << setw(15) << list[index]->nienKhoa << setw(10)
         << list[index]->hocKy << setw(8) << list[index]->nhom << setw(10)
         << list[index]->min << setw(10) << list[index]->max << setw(10)
         << list[index]->huy << setw(10) << list[index]->dsdk->getSoSVDK();
    SetColor();
}

void loadLTCToTable(LTC *list[], int length, int index) {
    ShowCur(false);
    clearTableContent();

    if (length == 0) {
        return;
    }

    index = index / MAX_TABLE_ROW * MAX_TABLE_ROW;

    int x = TABLE_X, y = TABLE_Y + 2;
    int currentPage = index / MAX_TABLE_ROW;
    int rowsLeft = length - currentPage * MAX_TABLE_ROW;
    rowsLeft = rowsLeft > MAX_TABLE_ROW ? MAX_TABLE_ROW : rowsLeft;

    showPageNumber(currentPage, length / MAX_TABLE_ROW);

    for (int i = 0; i < rowsLeft; i++) {
        gotoxy(x, y);
        cout << setfill(' ') << left << setw(15) << list[index + i]->maLTC
             << setw(15) << list[index + i]->maMH << setw(15)
             << list[index + i]->nienKhoa << setw(10) << list[index + i]->hocKy
             << setw(8) << list[index + i]->nhom << setw(10)
             << list[index + i]->min << setw(10) << list[index + i]->max
             << setw(10) << list[index + i]->huy << setw(10) << list[index + i]->dsdk->getSoSVDK();
        drawRow(x, y + 1, TABLE_WIDTH);
        y += 2;
    }
}

void printInsertLTCField(int index, string input) {
    ShowCur(false);
    SetColor();

    gotoxy(INSERT_X + LTC_FIELDS[index].length(), INSERT_Y + index * 2);
    cout << string(LTC_LIMITS[index], ' ');

    gotoxy(INSERT_X + LTC_FIELDS[index].length(), INSERT_Y + index * 2);
    cout << input;

    ShowCur(true);
}

void inputNkHk(string &nk, int &hk) {
    SetColor();
    clearDetail();
    clearNotification();

    nk = "";
    hk = -1;

    string input[] = {"", ""};
    unsigned fieldMaxIndex = 1;
    unsigned index = 0;
    unsigned count = input[0].length();
    unsigned key;

    gotoxy(INSERT_X, INSERT_Y - 1);
    cout << "Tim LTC trong nien khoa hoc ky";

    for (unsigned i = 0; i <= fieldMaxIndex; i++) {
        gotoxy(INSERT_X, INSERT_Y + (i + 1) * 2);
        cout << LTC_FIELDS[i + 1];
        printInsertLTCField(i + 1, input[i]);
    }

    gotoxy(INSERT_X + LTC_FIELDS[index + 1].length() + count,
           INSERT_Y + (index + 1) * 2);

    while (true) {
        key = _getch();

        // catch special input first
        if (key == 0 || key == 224) {
            key = _getch();
            if (key == KEY_UP) {
                index = index <= 0 ? fieldMaxIndex : index - 1;
                count = !input[index].empty() ? input[index].length() : 0;

                printInsertLTCField(index + 1, input[index]);
            } else if (key == KEY_DOWN) {
                index = index >= fieldMaxIndex ? 0 : index + 1;
                count = !input[index].empty() ? input[index].length() : 0;

                printInsertLTCField(index + 1, input[index]);
            } else if (key == KEY_LEFT) {
                count = count <= 0 ? input[index].length() : (count - 1);
                gotoxy(INSERT_X + LTC_FIELDS[index + 1].length() + count,
                       INSERT_Y + (index + 1) * 2);
            } else if (key == KEY_RIGHT) {
                count = count >= input[index].length() ? 0 : (count + 1);
                gotoxy(INSERT_X + LTC_FIELDS[index + 1].length() + count,
                       INSERT_Y + (index + 1) * 2);
            }
        } else if (key == BACKSPACE) {
            // if input is empty
            if (input[index].empty() || count == 0) {
                continue;
            }

            input[index].erase(count - 1, 1);
            count--;

            printInsertLTCField(index + 1, input[index]);
            gotoxy(INSERT_X + LTC_FIELDS[index + 1].length() + count,
                   INSERT_Y + (index + 1) * 2);
        } else if (key == ESC) {
            clearDetail();
            break;
        } else if (key == ENTER) {
            if (index == fieldMaxIndex) {
                clearNotification();
                bool pass = true;
                // check if one of the inputs is empty
                for (int i = 0; i < 2; i++) {
                    if (input[i].empty()) {
                        displayNotification("Hay dien day du thong tin.", RED);
                        pass = false;
                        break;
                    }
                }
                if (!pass) {
                    index = 0;
                    count = input[index].length();
                    printInsertLTCField(index + 1, input[index]);
                    continue;
                }

                if (!regex_match(input[0], NIEN_KHOA_REGEX)) {
                    displayNotification(
                        "Nien khoa khong dung dinh dang (VD: 2021-2022)");

                    index = 0;
                    count = input[index].length();
                    printInsertLTCField(index + 1, input[index]);

                    continue;
                } else if (!checkNienKhoa(input[0])) {
                    displayNotification(
                        "Nien khoa khong hop le (nam truoc be hon nam sau)");

                    index = 1;
                    count = input[index].length();
                    printInsertLTCField(index + 1, input[index]);

                    continue;
                }
                nk = input[0];
                hk = stoi(input[1]);
                return;
            } else {
                index++;
                count = input[index].length();
                printInsertLTCField(index + 1, input[index]);
            }
        } else {
            if (index == 0) {
                // Nien khoa
                if ((key >= '0' && key <= '9') || key == '-') {
                    if (input[index].length() >= LTC_LIMITS[1]) {
                        continue;
                    }
                    input[index].insert(count, 1, char(key));
                    count++;

                    printInsertLTCField(index + 1, input[index]);
                    gotoxy(INSERT_X + LTC_FIELDS[index + 1].length() + count,
                           INSERT_Y + (index + 1) * 2);
                }
            } else if (index == 1) {
                // hoc ky
                if (key >= '1' && key <= '9') {
                    if (input[index].length() >= LTC_LIMITS[2]) {
                        continue;
                    }
                    input[index].insert(count, 1, char(key));
                    count++;

                    printInsertLTCField(index + 1, input[index]);
                    gotoxy(INSERT_X + LTC_FIELDS[index + 1].length() + count,
                           INSERT_Y + (index + 1) * 2);
                }
            }
        }
    }
    nk = "";
    hk = -1;
}

int insertLTC(DsLTC &dsltc, DsMonHoc &dsmh) {
    clearDetail();
    gotoxy(INSERT_X, INSERT_Y - 1);
    cout << "THEM LTC";

    string input[] = {"", "", "0", "0", "0", "0", "0"};
    unsigned fieldMaxIndex = 6;
    unsigned index = 0;
    unsigned count = 0;
    unsigned key;

    for (unsigned i = 0; i <= fieldMaxIndex; i++) {
        gotoxy(INSERT_X, INSERT_Y + i * 2);
        cout << LTC_FIELDS[i];
        printInsertLTCField(i, input[i]);
    }

    gotoxy(INSERT_X + LTC_FIELDS[0].length() + count, INSERT_Y);

    while (true) {
        key = _getch();

        // catch special input first
        if (key == 0 || key == 224) {
            key = _getch();
            if (key == KEY_UP) {
                index = index <= 0 ? fieldMaxIndex : index - 1;
                count = !input[index].empty() ? input[index].length() : 0;

                printInsertLTCField(index, input[index]);
            } else if (key == KEY_DOWN) {
                index = index >= fieldMaxIndex ? 0 : index + 1;
                count = !input[index].empty() ? input[index].length() : 0;

                printInsertLTCField(index, input[index]);
            } else if (key == KEY_LEFT) {
                count = count <= 0 ? input[index].length() : (count - 1);
                gotoxy(INSERT_X + LTC_FIELDS[index].length() + count,
                       INSERT_Y + index * 2);
            } else if (key == KEY_RIGHT) {
                count = count >= input[index].length() ? 0 : (count + 1);
                gotoxy(INSERT_X + LTC_FIELDS[index].length() + count,
                       INSERT_Y + index * 2);
            }
        } else if (key == BACKSPACE) {
            // if input is empty
            if (input[index].empty() || count == 0) {
                continue;
            }

            input[index].erase(count - 1, 1);
            count--;
            printInsertLTCField(index, input[index]);
        } else if (key == ESC) {
            clearDetail();
            break;
        } else if (key == ENTER) {
            if (index == fieldMaxIndex) {
                clearNotification();
                // check if one of the inputs is empty
                bool pass = true;

                for (string s : input) {
                    if (s.empty()) {
                        displayNotification("Hay dien day du thong tin.", RED);
                        pass = false;
                        break;
                    }
                }
                if (!pass) {
                    index = 0;
                    printInsertLTCField(index, input[index]);
                    continue;
                }
                // check fields
                if (dsmh.search(input[0]) == NULL) {
                    // check mamh
                    displayNotification("MS mon hoc khong ton tai.", RED);
                    index = 0;
                    printInsertLTCField(index, input[index]);

                    continue;
                } else if (!regex_match(input[1], NIEN_KHOA_REGEX)) {
                    displayNotification(
                        "Nien khoa khong dung dinh dang (VD: 2021-2022)");

                    index = 1;
                    count = input[index].length();
                    printInsertLTCField(index, input[index]);

                    continue;
                } else if (!checkNienKhoa(input[1])) {
                    displayNotification(
                        "Nien khoa khong hop le (nam truoc lon hon nam sau)");

                    index = 1;
                    count = input[index].length();
                    printInsertLTCField(index, input[index]);

                    continue;
                } else if (stoi(input[4]) > stoi(input[5])) {
                    // check min > max
                    displayNotification("So sv min > so sv max. Hay nhap lai.",
                                        RED);
                    index = 4;
                    count = input[index].length();
                    printInsertLTCField(index, input[index]);

                    continue;
                }

                // kiểm tra khóa
                if (dsltc.search(input[0] + input[1] + input[2] + input[3])) {
                    displayNotification("LTC da ton tai");
                    continue;
                }

                // confirm insert
                if (showConfirmDialog("Xac nhan them? Y/N")) {
                    LTC *ltc = dsltc.insert(input[0], input[1], stoi(input[2]),
                                            stoi(input[3]), stoi(input[4]),
                                            stoi(input[5]), stoi(input[6]));
                    if (ltc) {
                        displayNotification("Luu lop tin chi thanh cong");
                        clearDetail();
                        dsltc.write();
                        return 1;
                    } else {
                        displayNotification("Luu lop tin chi khong thanh cong");

                        index = 0;
                        count = input[index].length();
                        printInsertLTCField(index, input[index]);
                    }
                } else {
                    clearNotification();
                    index = 0;
                    count = input[index].length();
                    printInsertLTCField(index, input[index]);

                    continue;
                }
            } else {
                index++;
                count = input[index].length();
                printInsertLTCField(index, input[index]);
            }
        } else {
            // catch character input
            printInsertLTCField(index, input[index]);

            if (index == 0) {
                // field is MAMH
                if ((key >= 'a' && key <= 'z') || (key >= 'A' && key <= 'Z') ||
                    (key >= '0' && key <= '9')) {
                    // out of range
                    if (input[index].length() >= LTC_LIMITS[index]) {
                        continue;
                    }
                    input[index].insert(count, 1, toupper(char(key)));
                    count++;

                    printInsertLTCField(index, input[index]);
                    gotoxy(INSERT_X + LTC_FIELDS[index].length() + count,
                           INSERT_Y + index * 2);
                }
            } else if (index == 1) {
                // Nien khoa
                if ((key >= '0' && key <= '9') || key == '-') {
                    if (input[index].length() >= LTC_LIMITS[index]) {
                        continue;
                    }
                    input[index].insert(count, 1, char(key));
                    count++;

                    printInsertLTCField(index, input[index]);
                    gotoxy(INSERT_X + LTC_FIELDS[index].length() + count,
                           INSERT_Y + index * 2);
                }
            } else if (index == 2 || index == 3 || index == 4 || index == 5) {
                // hoc ky / nhom / min / max
                if (key >= '0' && key <= '9') {
                    if (input[index].length() >= LTC_LIMITS[index]) {
                        continue;
                    }
                    input[index].insert(count, 1, char(key));
                    count++;
                    printInsertLTCField(index, input[index]);
                }
            } else if (index == 6) {
                // huy
                if (key == '0' || key == '1') {
                    if (input[index].length() >= LTC_LIMITS[index]) {
                        continue;
                    }
                    input[index].insert(count, 1, char(key));
                    count++;
                    printInsertLTCField(index, input[index]);
                    gotoxy(INSERT_X + LTC_FIELDS[index].length() + count,
                           INSERT_Y + index * 2);
                }
            }
        }
    }
    return 0;
}

int editLTC(DsLTC &dsltc, DsMonHoc &dsmh, LTC *ltc) {
    clearDetail();
    gotoxy(INSERT_X, INSERT_Y - 1);
    cout << "EDIT LTC";

    string oldKey = ltc->getKey();

    string input[] = {ltc->maMH,
                      ltc->nienKhoa,
                      to_string(ltc->hocKy),
                      to_string(ltc->nhom),
                      to_string(ltc->min),
                      to_string(ltc->max),
                      to_string(ltc->huy)};
    unsigned fieldMaxIndex = 6;
    unsigned index = 0;
    unsigned count = input[index].length();
    unsigned key;

    for (unsigned i = 0; i <= fieldMaxIndex; i++) {
        gotoxy(INSERT_X, INSERT_Y + i * 2);
        cout << LTC_FIELDS[i];
        printInsertLTCField(i, input[i]);
    }

    gotoxy(INSERT_X + LTC_FIELDS[index].length() + count, INSERT_Y);

    while (true) {
        key = _getch();

        // catch special input first
        if (key == 0 || key == 224) {
            key = _getch();
            if (key == KEY_UP) {
                index = index <= 0 ? fieldMaxIndex : index - 1;
                count = input[index].length();

                printInsertLTCField(index, input[index]);
            } else if (key == KEY_DOWN) {
                index = index >= fieldMaxIndex ? 0 : index + 1;
                count = input[index].length();

                printInsertLTCField(index, input[index]);
            } else if (key == KEY_LEFT) {
                count = count <= 0 ? input[index].length() : (count - 1);
                gotoxy(INSERT_X + LTC_FIELDS[index].length() + count,
                       INSERT_Y + index * 2);
            } else if (key == KEY_RIGHT) {
                count = count >= input[index].length() ? 0 : (count + 1);
                gotoxy(INSERT_X + LTC_FIELDS[index].length() + count,
                       INSERT_Y + index * 2);
            }
        } else if (key == BACKSPACE) {
            if (index == 0) {
                continue;
            }
            // if input is empty
            if (input[index].empty() || count == 0) {
                continue;
            }

            input[index].erase(count - 1, 1);
            count--;
            printInsertLTCField(index, input[index]);
            gotoxy(INSERT_X + LTC_FIELDS[index].length() + count,
                   INSERT_Y + index * 2);
        } else if (key == ESC) {
            clearDetail();
            break;
        } else if (key == ENTER) {
            if (index == fieldMaxIndex) {
                clearNotification();
                // check if one of the inputs is empty
                bool pass = true;

                for (string s : input) {
                    if (s.empty()) {
                        displayNotification("Hay dien day du thong tin.", RED);
                        pass = false;
                        break;
                    }
                }
                if (!pass) {
                    index = 0;
                    count = input[index].length();
                    printInsertLTCField(index, input[index]);
                    continue;
                }

                // check fields
                if (dsmh.search(input[0]) == NULL) {
                    // check mamh
                    displayNotification("Ma mon hoc khong ton tai.", RED);
                    index = 0;
                    count = input[index].length();
                    printInsertLTCField(index, input[index]);

                    continue;
                } else if (!regex_match(input[1], NIEN_KHOA_REGEX)) {
                    displayNotification(
                        "Nien khoa khong dung dinh dang (VD: 2021-2022)");

                    index = 1;
                    count = input[index].length();
                    printInsertLTCField(index, input[index]);

                    continue;
                } else if (!checkNienKhoa(input[1])) {
                    displayNotification(
                        "Nien khoa khong hop le (nam truoc lon hon nam sau)");

                    index = 1;
                    count = input[index].length();
                    printInsertLTCField(index, input[index]);

                    continue;
                } else if (stoi(input[4]) > stoi(input[5])) {
                    // check min > max
                    displayNotification("So sv min > so sv max. Hay nhap lai.",
                                        RED);
                    index = 4;
                    count = input[index].length();
                    printInsertLTCField(index, input[index]);

                    continue;
                }

                // check exist
                LTC *searchLTC =
                    dsltc.search(input[0] + input[1] + input[2] + input[3]);
                if (searchLTC) {
                    if (ltc->maLTC != searchLTC->maLTC) {
                        displayNotification("LTC da ton tai.", RED);
                        index = 0;
                        count = input[index].length();
                        printInsertLTCField(index, input[index]);

                        continue;
                    }
                }
                // confirm insert
                if (showConfirmDialog("Xac nhan sua? Y/N")) {
                    ltc->maMH = input[0];
                    ltc->nienKhoa = input[1];
                    ltc->hocKy = stoi(input[2]);
                    ltc->nhom = stoi(input[3]);

                    ltc->min = stoi(input[4]);
                    ltc->max = stoi(input[5]);
                    ltc->huy = stoi(input[6]);

                    displayNotification("Chinh sua lop tin chi thanh cong");
                    dsltc.write();
                    clearDetail();
                    return 1;
                } else {
                    clearNotification();

                    index = 0;
                    count = input[index].length();
                    printInsertLTCField(index, input[index]);
                }
            } else {
                clearNotification();
                index++;
                count = input[index].length();
                printInsertLTCField(index, input[index]);
            }
        } else {
            // catch character input
            if (index == 0) {
                // field is MAMH
                continue;
            } else if (index == 1) {
                // Nien khoa
                if ((key >= '0' && key <= '9') || key == '-') {
                    if (input[index].length() >= LTC_LIMITS[index]) {
                        continue;
                    }
                    input[index].insert(count, 1, char(key));
                    count++;

                    printInsertLTCField(index, input[index]);
                    gotoxy(INSERT_X + LTC_FIELDS[index].length() + count,
                           INSERT_Y + index * 2);
                }
            } else if (index == 2 || index == 3 || index == 4 || index == 5) {
                // hoc ky / nhom / min / max
                if (key >= '0' && key <= '9') {
                    if (input[index].length() >= LTC_LIMITS[index]) {
                        continue;
                    }
                    input[index].insert(count, 1, char(key));
                    count++;

                    printInsertLTCField(index, input[index]);
                    gotoxy(INSERT_X + LTC_FIELDS[index].length() + count,
                           INSERT_Y + index * 2);
                }
            } else if (index == 6) {
                // huy
                if (key == '0' || key == '1') {
                    if (input[index].length() >= LTC_LIMITS[index]) {
                        continue;
                    }
                    input[index].insert(count, 1, char(key));
                    count++;

                    printInsertLTCField(index, input[index]);
                    gotoxy(INSERT_X + LTC_FIELDS[index].length() + count,
                           INSERT_Y + index * 2);
                }
            }
        }
    }
    return 0;
}

int huyLTC(DsLTC &dsltc) {
    clearTableContent();
    clearInfo();

    string nienkhoa;
    int hocky;

    while (nienkhoa.empty() || hocky == -1) {
        displayNotification("Hay nhap lai nien khoa hoc ky");
        inputNkHk(nienkhoa, hocky);
    }

    LTC *list[1000];
    int len = 0;

    dsltc.filterLtcCoTheHuy(list, nienkhoa, hocky, len);

    if (len == 0) {
        displayNotification("Khong co lop nao can huy trong nien khoa " + nienkhoa + " hoc ky " + to_string(hocky));
        return 0;
    }

    loadLTCToTable(list, len, 0);

    if (showConfirmDialog("Xac nhan huy cac lop tin chi trong danh sach? Y/N")) {
        for (int i = 0; i < len; i++) {
            list[i]->huy = true;
        }
        displayNotification("Huy thanh cong " + to_string(len) + " lop");
        Sleep(500);

        return 1;
    } else {
        return 0;
    }

    return 0;
}

int initLTCTab(DsMonHoc &dsmh, DSSV &dssv, DsLTC &dsltc) {
    clearTab();
    SetColor(BLACK, RED);

    gotoxy(TABLE_X, TABLE_Y);
    cout << "Ma LTC";
    gotoxy(TABLE_X + 15, TABLE_Y);
    cout << "Ma MH";
    gotoxy(TABLE_X + 30, TABLE_Y);
    cout << "Nien khoa";
    gotoxy(TABLE_X + 45, TABLE_Y);
    cout << "Hoc ky";
    gotoxy(TABLE_X + 55, TABLE_Y);
    cout << "Nhom";
    gotoxy(TABLE_X + 63, TABLE_Y);
    cout << "SV min";
    gotoxy(TABLE_X + 73, TABLE_Y);
    cout << "SV max";
    gotoxy(TABLE_X + 83, TABLE_Y);
    cout << "Huy";
    gotoxy(TABLE_X + 93, TABLE_Y);
    cout << "Da dky";
    SetColor();

    drawRow(TABLE_X, TABLE_Y + 1, TABLE_WIDTH);

    gotoxy(TABLE_X, LAST_ROW + 3);
    cout << "CTRL + D de huy cac lop chua du sinh vien dang ky";

    int key;
    // số hàng còn lại
    int nOfRowRemains;
    // index trong list
    int index = 0;
    // current page
    int currentPage;
    // max number of pages
    int nPage;

    if (dsltc.count > 0) {
        loadLTCToTable(dsltc.dsltc, dsltc.count, index);
        highlightIndex(dsltc.dsltc, index);
    }

    while (true) {
        currentPage = index / MAX_TABLE_ROW;
        nPage = dsltc.count / MAX_TABLE_ROW;
        nOfRowRemains = dsltc.count - currentPage * MAX_TABLE_ROW;
        nOfRowRemains =
            nOfRowRemains > MAX_TABLE_ROW ? MAX_TABLE_ROW : nOfRowRemains;

        key = _getch();

        if (key == 0 || key == 224) {
            if (key == 0) {
                key = _getch();

                if (key >= KEY_F1 && key <= KEY_F8) {
                    return key;
                } else if (key == ALT_F4) {
                    clearTab();
                    return ALT_F4;
                }
            } else if (key == 224) {
                key = _getch();

                if (key == KEY_UP) {
                    if (dsltc.count == 0) {
                        continue;
                    }
                    // first row -> last row
                    if (index <= currentPage * MAX_TABLE_ROW) {
                        dehighlightIndex(dsltc.dsltc, index);
                        index = currentPage * MAX_TABLE_ROW + nOfRowRemains - 1;
                        highlightIndex(dsltc.dsltc, index);
                    } else {
                        index--;
                        highlightIndex(dsltc.dsltc, index);
                        dehighlightIndex(dsltc.dsltc, index + 1);
                    }
                } else if (key == KEY_DOWN) {
                    if (dsltc.count == 0) {
                        continue;
                    }

                    if (index >= currentPage * MAX_TABLE_ROW + nOfRowRemains - 1) {
                        dehighlightIndex(dsltc.dsltc, index);
                        index = currentPage * MAX_TABLE_ROW;
                        highlightIndex(dsltc.dsltc, index);
                    } else {
                        index++;
                        highlightIndex(dsltc.dsltc, index);
                        dehighlightIndex(dsltc.dsltc, index - 1);
                    }
                } else if (key == KEY_LEFT) {
                    if (dsltc.count == 0) {
                        continue;
                    }

                    index = (currentPage > 0 ? currentPage - 1 : nPage) * MAX_TABLE_ROW;

                    loadLTCToTable(dsltc.dsltc, dsltc.count, index);
                    highlightIndex(dsltc.dsltc, index);
                } else if (key == KEY_RIGHT) {
                    if (dsltc.count == 0) {
                        continue;
                    }

                    currentPage = currentPage >= nPage ? 0 : currentPage + 1;
                    index = currentPage * MAX_TABLE_ROW;
                    loadLTCToTable(dsltc.dsltc, dsltc.count, index);
                    highlightIndex(dsltc.dsltc, index);
                } else if (key == INSERT) {
                    // insert
                    if (insertLTC(dsltc, dsmh)) {
                        index = 0;
                        loadLTCToTable(dsltc.dsltc, dsltc.count, index);
                        highlightIndex(dsltc.dsltc, index);
                    } else if (dsltc.count == 0) {
                        clearTableContent();
                        continue;
                    }

                    loadLTCToTable(dsltc.dsltc, dsltc.count, index);
                    highlightIndex(dsltc.dsltc, index);
                } else if (key == DEL) {
                    // remove
                    if (dsltc.count == 0) {
                        continue;
                    }

                    if (showConfirmDialog("Xac nhan xoa LTC? Y/N")) {
                        if (dsltc.remove(dsltc.dsltc[index]->maLTC)) {
                            dsltc.write();
                            displayNotification("Xoa ltc thanh cong");

                            if (dsltc.count == 0) {
                                clearTableContent();
                                continue;
                            }

                            index = 0;
                            loadLTCToTable(dsltc.dsltc, dsltc.count, index);
                            highlightIndex(dsltc.dsltc, index);
                        } else {
                            displayNotification("Xoa ltc k thanh cong");
                        }
                    } else {
                        clearNotification();
                    }
                }
            }
        } else if (key == CTRL_D) {
            // liet danh sach cac lop chua du sinh vien dk
            if (dsltc.count == 0) {
                continue;
            }

            // ct hủy ltc
            if (!huyLTC(dsltc)) {
                clearNotification();

                loadLTCToTable(dsltc.dsltc, dsltc.count, index);
                highlightIndex(dsltc.dsltc, index);
                continue;
            }
            clearNotification();

            // index = 0;
            loadLTCToTable(dsltc.dsltc, dsltc.count, index);
            highlightIndex(dsltc.dsltc, index);
        } else if (key == ENTER) {
            if (dsltc.count == 0) {
                continue;
            }
            // edit ltc
            editLTC(dsltc, dsmh, dsltc.dsltc[index]);

            // index = 0;
            loadLTCToTable(dsltc.dsltc, dsltc.count, index);
            highlightIndex(dsltc.dsltc, index);
        }
    }
    return 0;
}

#endif