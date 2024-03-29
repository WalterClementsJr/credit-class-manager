#ifndef TAB_DK_H
#define TAB_DK_H

#include <iomanip>
#include <iostream>
#include <regex>
#include <string>

#include "model/DangKy.h"
#include "model/DsMonHoc.h"
#include "model/LTC.h"
#include "ui/TabLTC.h"
#include "ui/drawing.h"
#include "util/Helper.h"

using std::cout;
using std::endl;
using std::string;
using std::setfill;
using std::setw;
using std::left;

void highlightIndex(LTC *list[], string *tenMH, int index) {
    SetColor(BLACK, BLUE);
    ShowCursor(false);

    int sosv = list[index]->dsdk->getSoSVDK();

    gotoxy(TABLE_X, TABLE_Y + 2 + (index % MAX_TABLE_ROW) * 2);
    cout << setfill(' ') << left << setw(15) << list[index]->maMH << setw(60)
         << tenMH[index] << setw(10) << list[index]->nhom << setw(10) << sosv
         << setw(5) << list[index]->max - sosv;

    SetColor();
}

void dehighlightIndex(LTC *list[], string *tenMH, int index) {
    SetColor();
    ShowCursor(false);

    int sosv = list[index]->dsdk->getSoSVDK();

    gotoxy(TABLE_X, TABLE_Y + 2 + (index % MAX_TABLE_ROW) * 2);
    cout << setfill(' ') << left << setw(15) << list[index]->maMH << setw(60)
         << tenMH[index] << setw(10) << list[index]->nhom << setw(10) << sosv
         << setw(5) << list[index]->max - sosv;
}

// display MonHoc to table based on index
void loadThongTinLTCToTable(LTC *list[], string *tenMH, int length, int index) {
    ShowCur(false);
    clearTableContent();

    if (length == 0) {
        return;
    }
    index = index / MAX_TABLE_ROW * MAX_TABLE_ROW;

    int x = TABLE_X, y = TABLE_Y + 2;
    int currentPage = index / MAX_TABLE_ROW;
    showPageNumber(currentPage, length / MAX_TABLE_ROW);

    int rowsLeft = length - currentPage * MAX_TABLE_ROW;
    rowsLeft = rowsLeft > MAX_TABLE_ROW ? MAX_TABLE_ROW : rowsLeft;

    int sosv = 0;
    for (int i = 0; i < rowsLeft; i++) {
        gotoxy(x, y);
        sosv = list[index + i]->dsdk->getSoSVDK();
        cout << setfill(' ') << left << setw(15) << list[index + i]->maMH
             << setw(60) << tenMH[index + i] << setw(10)
             << list[index + i]->nhom << setw(10) << sosv << setw(5)
             << list[index + i]->max - sosv;
        drawRow(x, y + 1, TABLE_WIDTH);
        y += 2;
    }
}

int initDKTab(DsMonHoc &dsmh, DSSV &dssv, DsLTC &dsltc) {
    clearTab();
    SetColor(BLACK, RED);

    gotoxy(TABLE_X, TABLE_Y);
    cout << "Ma MH";
    gotoxy(TABLE_X + 15, TABLE_Y);
    cout << "Ten mon hoc";
    gotoxy(TABLE_X + 75, TABLE_Y);
    cout << "Nhom";
    gotoxy(TABLE_X + 85, TABLE_Y);
    cout << "Da dk";
    gotoxy(TABLE_X + 95, TABLE_Y);
    cout << "So slot";
    SetColor();
    drawRow(TABLE_X, TABLE_Y + 1, TABLE_WIDTH);

    gotoxy(TABLE_X, LAST_ROW + 1);
    cout << "CTRL + F: nhap masv. CTRL + A: nhap nien khoa hoc ky";
    gotoxy(TABLE_X, LAST_ROW + 2);
    cout << "Tab: xem ds ltc co the dk/da dk";

    int key;
    int index = 0;
    int nOfRowRemains;
    int currentPage, nPage;
    int dsLength = 0;

    string mssv = "";
    string nienkhoa = "";
    int hocky = -1;
    // chuyen ds
    bool xemDSChuaDK = true;

    LTC *list[1000];
    LTC *tempList[1000];
    string *tenMH = new string[1000];

    while (true) {
        currentPage = index / MAX_TABLE_ROW;
        nPage = dsLength / MAX_TABLE_ROW;
        nOfRowRemains = dsLength - currentPage * MAX_TABLE_ROW;
        nOfRowRemains =
            nOfRowRemains > MAX_TABLE_ROW ? MAX_TABLE_ROW : nOfRowRemains;

        key = _getch();

        if (key == 0 || key == 224) {
            if (key == 0) {
                key = _getch();
                if (key >= KEY_F1 && key <= KEY_F8) {
                    delete[] tenMH;

                    return key;
                } else if (key == ALT_F4) {
                    clearTab();
                    delete[] tenMH;
                    return ALT_F4;
                }
            } else if (key == 224) {
                key = _getch();

                if (key == KEY_UP) {
                    if (dsLength == 0) {
                        continue;
                    }
                    if (index <= currentPage * MAX_TABLE_ROW) {
                        dehighlightIndex(list, tenMH, index);
                        index = currentPage * MAX_TABLE_ROW + nOfRowRemains - 1;
                        highlightIndex(list, tenMH, index);
                    } else {
                        index--;
                        highlightIndex(list, tenMH, index);
                        dehighlightIndex(list, tenMH, index + 1);
                    }
                } else if (key == KEY_DOWN) {
                    if (dsLength == 0) {
                        continue;
                    }
                    if (index >=
                        currentPage * MAX_TABLE_ROW + nOfRowRemains - 1) {
                        dehighlightIndex(list, tenMH, index);
                        index = currentPage * MAX_TABLE_ROW;
                        highlightIndex(list, tenMH, index);
                    } else {
                        index++;
                        highlightIndex(list, tenMH, index);
                        dehighlightIndex(list, tenMH, index - 1);
                    }
                } else if (key == KEY_LEFT) {
                    // prev page
                    if (dsLength == 0) {
                        continue;
                    }
                    index = (currentPage > 0 ? currentPage - 1 : nPage) *
                            MAX_TABLE_ROW;

                    loadThongTinLTCToTable(list, tenMH, dsLength, index);
                    highlightIndex(list, tenMH, index);
                } else if (key == KEY_RIGHT) {
                    if (dsLength == 0) {
                        continue;
                    }
                    // next page
                    currentPage = currentPage >= nPage ? 0 : currentPage + 1;
                    index = currentPage * MAX_TABLE_ROW;

                    loadThongTinLTCToTable(list, tenMH, dsLength, index);
                    highlightIndex(list, tenMH, index);
                } else if (key == INSERT) {
                }
            }
        } else if (key == CTRL_F) {
            string temp = inputString("Nhap MSSV: ", 10, mssv);
            if (temp.empty()) {
                continue;
            }
            NodeSinhVien *nsv = dssv.search(temp);

            if (!nsv) {
                displayNotification("MSSV khong ton tai.");
                continue;
            }
            mssv = temp;

            clearInfo();
            gotoxy(TABLE_X, LAST_ROW + 4);
            cout << nsv->sinhVien.maSV + " "
                 << nsv->sinhVien.ho + " " + nsv->sinhVien.ten;
            gotoxy(TABLE_X, LAST_ROW + 5);
            cout << "Gioi tinh: " << nsv->sinhVien.phai
                 << ", SDT: " << nsv->sinhVien.soDT;
            gotoxy(TABLE_X, LAST_ROW + 6);
            cout << "Lop: " + nsv->sinhVien.maLop;
        } else if (key == CTRL_A) {
            if (mssv.empty()) {
                displayNotification("Hay nhap mssv truoc");
                continue;
            }
            // nhap nien khoa hk
            while (nienkhoa.empty() || hocky == -1) {
                displayNotification("Hay nhap lai nien khoa hoc ky");
                inputNkHk(nienkhoa, hocky);
            }

            if (soSanhNienKhoa(nienkhoa, getSoNamTuMaSV(mssv)) > 0) {
                displayNotification("Sinh vien khong the dang ky cac lop trong nien khoa nay");
                continue;
            }

            xemDSChuaDK = true;
            dsltc.filterLtcTheoNkHk(list, mssv, nienkhoa, hocky, dsLength, xemDSChuaDK ? 2 : 0);

            showNote(xemDSChuaDK ? "DS LTC co the dk" : "DS LTC da dk");

            if (dsLength == 0) {
                displayNotification("Khong co lop");
                continue;
            }

            for (int i = 0; i < dsLength; i++) {
                MonHoc *mh = dsmh.search(list[i]->maMH);
                if (mh) {
                    tenMH[i] = mh->ten;
                } else {
                    tenMH[i] = "???";
                }
            }
            index = 0;
            loadThongTinLTCToTable(list, tenMH, dsLength, index);
            highlightIndex(list, tenMH, index);
        } else if (key == TAB) {
            clearNotification();

            if (mssv.empty()) {
                displayNotification("Hay nhap MSSV");
                continue;
            } else if (nienkhoa.empty() || hocky == -1) {
                displayNotification("Hay nhap nien khoa, hoc ky");
                continue;
            }

            xemDSChuaDK = !xemDSChuaDK;

            dsltc.filterLtcTheoNkHk(tempList, mssv, nienkhoa, hocky, dsLength,
                                    xemDSChuaDK ? 2 : 0);
            showNote(xemDSChuaDK ? "DS LTC co the dk" : "DS LTC da dk");

            if (dsLength == 0) {
                clearTableContent();
                displayNotification("Khong co lop");
                continue;
            }
            for (int i = 0; i < dsLength; i++) {
                list[i] = tempList[i];
            }

            for (int i = 0; i < dsLength; i++) {
                MonHoc *mh = dsmh.search(list[i]->maMH);
                if (mh) {
                    tenMH[i] = mh->ten;
                } else {
                    tenMH[i] = "???";
                }
            }

            index = 0;
            loadThongTinLTCToTable(list, tenMH, dsLength, index);
            highlightIndex(list, tenMH, index);
        } else if (key == ENTER) {
            if (dsLength == 0) {
                continue;
            }
            if (showConfirmDialog("Xac nhan " + string(xemDSChuaDK ? "" : "huy ") +
                                  string("dang ky lop ") + tenMH[index] + string("? Y/N"))) {
                // huy dk/dk
                list[index]->dsdk->insertOrder(mssv, 0, !xemDSChuaDK);
                dsltc.write();

                // reload data
                dsltc.filterLtcTheoNkHk(tempList, mssv, nienkhoa, hocky, dsLength, xemDSChuaDK ? 2 : 0);
                showNote(xemDSChuaDK ? "DS LTC co the dk" : "DS LTC da dk");

                if (dsLength == 0) {
                    clearTableContent();
                    displayNotification("Khong co lop");
                    continue;
                }
                for (int i = 0; i < dsLength; i++) {
                    list[i] = tempList[i];
                }

                for (int i = 0; i < dsLength; i++) {
                    MonHoc *mh = dsmh.search(list[i]->maMH);
                    if (mh) {
                        tenMH[i] = mh->ten;
                    } else {
                        tenMH[i] = "???";
                    }
                }

                index = 0;
                loadThongTinLTCToTable(list, tenMH, dsLength, index);
                highlightIndex(list, tenMH, index);
            }
            clearNotification();
        }
    }
    delete[] tenMH;
    return 0;
}

#endif
