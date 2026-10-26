#include "rallytrophy.h"

unsigned short DAT_00577bc0;
unsigned int DAT_00577bc4;

// FUNCTION: RALLYTROPHY 0x4a1820
// Decodes the stored product key into "XXXX-XXXX-XXXX-XXXX".
const char* FUN_004a1820(void)
{
    DAT_0056ae78[4] = '-';
    DAT_0056ae78[9] = '-';
    DAT_0056ae78[14] = '-';
    DAT_0056ae78[19] = 0;
    for (int i = 0; i < 4; i++) {
        DAT_0056ae78[i] = (DAT_0056b0d8[1 + i] ^ DAT_0056b0d8[0]) + i * 7 + 0x6e;
        DAT_0056ae78[5 + i] = (DAT_0056b0d8[6 + i] + i * 13 - 0x5f) ^ DAT_0056b0d8[5];
        DAT_0056ae78[10 + i] = (DAT_0056b0d8[11 + i] ^ DAT_0056b0d8[10]) + i * 23 + 0x4c;
        DAT_0056ae78[15 + i] = (DAT_0056b0d8[16 + i] + i * 54 - 0x16) ^ DAT_0056b0d8[15];
    }
    return DAT_0056ae78;
}

// FUNCTION: RALLYTROPHY 0x4a18d0
// Frame limiter: owes `microseconds` of waiting, sleeps it off in whole
// milliseconds once more than 1 ms is owed and carries the overshoot forward.
void FUN_004a18d0(int microseconds)
{
    if (!DAT_0056b0f4) {
        DAT_0056b0f4 = 1;
        QueryPerformanceFrequency(&DAT_00501530);
    }
    DAT_0056b0ec += microseconds;
    int owed = DAT_0056b0ec - DAT_0056b0f0;
    if (owed > 1000) {
        LARGE_INTEGER start, end;
        DAT_0056b0ec = owed;
        QueryPerformanceCounter(&start);
        Sleep(DAT_0056b0ec / 1000);
        QueryPerformanceCounter(&end);
        end.QuadPart = (unsigned __int64)((end.QuadPart - start.QuadPart) * 1000000) / DAT_00501530.QuadPart;
        DAT_0056b0f0 = end.LowPart - DAT_0056b0ec;
        DAT_0056b0ec %= 1000;
    }
}

// FUNCTION: RALLYTROPHY 0x4a1ad0
// Stores the directory containing RallyTrophy.exe in the registry as "HDPath".
void FUN_004a1ad0(void)
{
    int i;

    DAT_0056b300[0] = 0;
    GetModuleFileNameA(0, DAT_0056b300, sizeof(DAT_0056b300));

    for (i = strlen(DAT_0056b300) - 1; i > 0; i--) {
        if (DAT_0056b300[i] == '/' || DAT_0056b300[i] == '\\') {
            break;
        }
    }
    if (i > 0) {
        DAT_0056b300[i] = 0;
    }

    FUN_004b7750(DAT_0056b0a8, DAT_00501634, DAT_0056b300);
}

// FUNCTION: RALLYTROPHY 0x4a1b40
int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR commandLine, int showCommand)
{
    srand(timeGetTime());
    RegCreateKeyExA(HKEY_LOCAL_MACHINE, "Software\\Bugbear\\Rally Trophy", 0, NULL, 0, KEY_ALL_ACCESS, NULL, &DAT_0056b0a8, NULL);
    FUN_004a1ad0();
    if (commandLine && (!_stricmp(commandLine, "setup") || !_stricmp(commandLine, "-setup"))) {
        DAT_0056b998 = 1;
    }
    CoInitializeEx(NULL, 0);
    DAT_0056b0c4 = new CommandLine(commandLine);
    for (int i = 0; i < DAT_0056b0c4->GetCount(); i++) {
        if (!_stricmp(DAT_0056b0c4->GetArg(i), "/s")) {
            DAT_00501471 = 1;
        } else if (!_stricmp(DAT_0056b0c4->GetArg(i), "/r")) {
            DAT_00501472 = 1;
        }
    }
    DAT_0056b98c = LoadIconA(instance, MAKEINTRESOURCE(0x66));
    FUN_004a7d30();

    try {
        try {
            remove("data/highscores/sessionscores.hs");
            FUN_00404d50();
            FUN_004a4440("data/device.cfg");
            FUN_004a4660("data/options.cfg");
            if (!DAT_0056b998 && !(DAT_0056b0d8[0] & 1) && !FUN_004a4250(instance)) {
                return -1;
            }
            int result = FUN_004a1fe0(instance);
            if (DAT_0056b998) {
                if (result == 1) {
                    FUN_004a45c0("data/device.cfg", 1);
                }
            } else if (result == -1) {
                MessageBoxA(NULL, "No suitable 3d-devices found", "ERROR", MB_ICONHAND);
            } else if (result == 1 && DAT_0056b980->FUN_004bbff0(DAT_0056b968[DAT_0056b984].adapter) && FUN_004a3390(instance)) {
                if (DAT_0056b994) {
                    FUN_004a49b0();
                }
                FUN_004a45c0("data/device.cfg", 1);
                FUN_004a3ca0(commandLine);
            }
        } catch (char* e) {
            FUN_00402c90("other exception %s", e);
            FUN_004a3a00();
            throw;
        } catch (int e) {
            FUN_00402c90("integer exception %i", e);
            FUN_004a3a00();
            throw;
        } catch (assertion_error& e) {
            FUN_00402c90("assertion failure");
            FUN_00402c90(e.what());
            FUN_004a3a00();
            throw;
        } catch (file_error& e) {
            FUN_00402c90("file error");
            FUN_00402c90(e.what());
            FUN_004a3a00();
            throw;
        } catch (std::logic_error& e) {
            FUN_00402c90("logic error");
            FUN_00402c90(e.what());
            FUN_004a3a00();
            throw;
        } catch (std::runtime_error& e) {
            FUN_00402c90("runtime error: %s", e.what());
            FUN_004a3a00();
            throw;
        } catch (exception& e) {
            FUN_00402c90("exception: %s", e.what());
            FUN_004a3a00();
            throw;
        } catch (...) {
            FUN_00402c90("exception catched");
            FUN_004a3a00();
            throw;
        }
    } catch (file_error& e) {
        FUN_00402c90("file error");
        FUN_00402c90(e.what());
        MessageBoxA(NULL, e.what(), "ERROR", MB_ICONHAND);
    } catch (std::runtime_error& e) {
        FUN_00402c90("runtime error");
        FUN_00402c90(e.what());
        MessageBoxA(NULL, e.what(), "ERROR", MB_ICONHAND);
    } catch (std::logic_error& e) {
        FUN_00402c90("logic error");
        FUN_00402c90(e.what());
        MessageBoxA(NULL, e.what(), "ERROR", MB_ICONHAND);
    } catch (exception& e) {
        FUN_00402c90("unknown exception");
        FUN_00402c90(e.what());
        MessageBoxA(NULL, e.what(), "ERROR", MB_ICONHAND);
    }
    FUN_004a3a00();
    CoUninitialize();
    if (DAT_0056b0c4) {
        delete DAT_0056b0c4;
    }
    return 0;
}

// FUNCTION: RALLYTROPHY 0x4a1fe0
// Loads the renderer plugin, lists its adapters (preselecting the saved display
// mode) and runs the setup dialog unless the saved settings can be used as is.
// Returns 1 to start the game, -1 if no plugin could be loaded.
int FUN_004a1fe0(HINSTANCE instance)
{
    DAT_0056b980 = new T_56b980();
    DAT_0056b0b0 = new T_56b0b0();
    if (!DAT_0056b980->FUN_004bbf60("device/bbdx8.dll")) {
        return -1;
    }
    AdapterNode* node = DAT_0056b980->FUN_00403c90()->m_head;
    int width = DAT_0056b8c8.width;
    int height = DAT_0056b8c8.height;
    int bpp = DAT_0056b8c8.bpp;
    while (node) {
        AdapterEntry entry;
        entry.antialias = DAT_0056b8c8.antialias;
        entry.adapter = node->data;
        entry.modeIndex = 0;
        entry.mode = 0;
        entry.refresh = DAT_0056b8c8.refresh;
        std::vector<DisplayMode*>* modes = entry.adapter->GetModes();
        for (unsigned int i = 0; i < modes->size(); i++) {
            DisplayMode* mode = (*modes)[i];
            if (mode->width == width && mode->height == height && mode->bpp == bpp) {
                entry.modeIndex = i;
                entry.mode = (*modes)[i];
                break;
            }
        }
        if (!entry.mode && !modes->empty()) {
            entry.mode = (*modes)[0];
        }
        DAT_0056b968.Add(entry);
        node = node->next;
    }

    int result = 1;
    if (DAT_0056b990 || DAT_0056b998 || !FUN_004a2270()) {
        DAT_0056b964 = LoadImageA(instance, MAKEINTRESOURCE(0x6a), 0, 0, 0, 0);
        DAT_0056b960 = LoadImageA(instance, MAKEINTRESOURCE(0x6b), 0, 0, 0, 0);
        result = DialogBoxParamA(instance, MAKEINTRESOURCE(0x65), NULL, FUN_004a2440, 0) != IDCANCEL;
        DeleteObject(DAT_0056b964);
        DeleteObject(DAT_0056b960);
    }
    return result;
}

// FUNCTION: RALLYTROPHY 0x4a2440
// Keep declaration order, but initialize found after binding devices: VC6
// assigns stack slots by declaration and schedules stores by assignment.
// Setup dialog: adapter, display mode, sound device and quality options.
BOOL CALLBACK FUN_004a2440(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_INITDIALOG) {
        int item;
        DAT_0056b0f8 = 0;
        SendDlgItemMessageA(dialog, 1000, CB_RESETCONTENT, 0, 0);

        bool found;
        std::vector<DeviceInfo>& devices = DAT_0056b0b0->m_devices;
        found = false;
        unsigned int i;
        for (i = 0; i < devices.size(); i++) {
            item = SendDlgItemMessageA(dialog, 0x3f2, CB_ADDSTRING, 0, (LPARAM)devices[i].name);
            SendDlgItemMessageA(dialog, 0x3f2, CB_SETITEMDATA, item, i);
            if (!strcmp(devices[i].name, DAT_0056b8c8.device)) {
                found = true;
                DAT_0056b988 = i;
            }
        }
        if (!found) {
            DAT_0056b988 = DAT_0056b0b0->m_default;
        }

        for (i = 0; (int)i < DAT_0056b968.m_count; i++) {
            item = SendDlgItemMessageA(dialog, 1000, CB_ADDSTRING, 0, (LPARAM)DAT_0056b968[i].adapter->GetName());
            SendDlgItemMessageA(dialog, 1000, CB_SETITEMDATA, item, i);
            if (!memcmp(DAT_0056b968[i].adapter->GetGuid(), &DAT_0056b8c8.adapter, sizeof(GUID))) {
                DAT_0056b984 = i;
            }
        }
        SendDlgItemMessageA(dialog, 1000, CB_SETCURSEL, DAT_0056b984, 0);
        SendDlgItemMessageA(dialog, 0x3f2, CB_SETCURSEL, DAT_0056b988, 0);
        SendDlgItemMessageA(dialog, 0x3eb, BM_SETCHECK, DAT_0056b8c8.unk7c != 0, 0);
        SendDlgItemMessageA(dialog, 0x3fa, BM_SETCHECK, DAT_0056b8c8.unk7d != 0, 0);

        item = SendDlgItemMessageA(dialog, 0x3f6, CB_ADDSTRING, 0, (LPARAM)"2 Speakers");
        SendDlgItemMessageA(dialog, 0x3f6, CB_SETITEMDATA, item, 0);
        item = SendDlgItemMessageA(dialog, 0x3f6, CB_ADDSTRING, 0, (LPARAM)"Headphones");
        SendDlgItemMessageA(dialog, 0x3f6, CB_SETITEMDATA, item, 1);
        item = SendDlgItemMessageA(dialog, 0x3f6, CB_ADDSTRING, 0, (LPARAM)"Surround");
        SendDlgItemMessageA(dialog, 0x3f6, CB_SETITEMDATA, item, 2);
        item = SendDlgItemMessageA(dialog, 0x3f6, CB_ADDSTRING, 0, (LPARAM)"4 Speakers");
        SendDlgItemMessageA(dialog, 0x3f6, CB_SETITEMDATA, item, 3);
        SendDlgItemMessageA(dialog, 0x3f6, CB_SETCURSEL, DAT_0056b8c8.speakers, 0);
        item = SendDlgItemMessageA(dialog, 0x404, CB_ADDSTRING, 0, (LPARAM)"11 khz");
        SendDlgItemMessageA(dialog, 0x404, CB_SETITEMDATA, item, 0);
        item = SendDlgItemMessageA(dialog, 0x404, CB_ADDSTRING, 0, (LPARAM)"22 khz");
        SendDlgItemMessageA(dialog, 0x404, CB_SETITEMDATA, item, 1);
        item = SendDlgItemMessageA(dialog, 0x404, CB_ADDSTRING, 0, (LPARAM)"44 khz");
        SendDlgItemMessageA(dialog, 0x404, CB_SETITEMDATA, item, 2);
        SendDlgItemMessageA(dialog, 0x404, CB_SETCURSEL, DAT_0056b8c8.sampleRate, 0);
        item = SendDlgItemMessageA(dialog, 0x406, CB_ADDSTRING, 0, (LPARAM)"8");
        SendDlgItemMessageA(dialog, 0x406, CB_SETITEMDATA, item, 0);
        item = SendDlgItemMessageA(dialog, 0x406, CB_ADDSTRING, 0, (LPARAM)"16");
        SendDlgItemMessageA(dialog, 0x406, CB_SETITEMDATA, item, 1);
        item = SendDlgItemMessageA(dialog, 0x406, CB_ADDSTRING, 0, (LPARAM)"24");
        SendDlgItemMessageA(dialog, 0x406, CB_SETITEMDATA, item, 2);
        item = SendDlgItemMessageA(dialog, 0x406, CB_ADDSTRING, 0, (LPARAM)"32");
        SendDlgItemMessageA(dialog, 0x406, CB_SETITEMDATA, item, 3);
        item = SendDlgItemMessageA(dialog, 0x406, CB_ADDSTRING, 0, (LPARAM)"40");
        SendDlgItemMessageA(dialog, 0x406, CB_SETITEMDATA, item, 4);
        item = SendDlgItemMessageA(dialog, 0x406, CB_ADDSTRING, 0, (LPARAM)"48");
        SendDlgItemMessageA(dialog, 0x406, CB_SETITEMDATA, item, 5);
        item = SendDlgItemMessageA(dialog, 0x406, CB_ADDSTRING, 0, (LPARAM)"56");
        SendDlgItemMessageA(dialog, 0x406, CB_SETITEMDATA, item, 6);
        item = SendDlgItemMessageA(dialog, 0x406, CB_ADDSTRING, 0, (LPARAM)"64");
        SendDlgItemMessageA(dialog, 0x406, CB_SETITEMDATA, item, 7);
        SendDlgItemMessageA(dialog, 0x406, CB_SETCURSEL, DAT_0056b8c8.voices, 0);
        item = SendDlgItemMessageA(dialog, 0x3ed, CB_ADDSTRING, 0, (LPARAM)"High (32 Bits)");
        SendDlgItemMessageA(dialog, 0x3ed, CB_SETITEMDATA, item, 1);
        item = SendDlgItemMessageA(dialog, 0x3ed, CB_ADDSTRING, 0, (LPARAM)"Medium (16 Bits)");
        SendDlgItemMessageA(dialog, 0x3ed, CB_SETITEMDATA, item, 0);
        item = SendDlgItemMessageA(dialog, 0x3ed, CB_ADDSTRING, 0, (LPARAM)"Low (Compressed)");
        SendDlgItemMessageA(dialog, 0x3ed, CB_SETITEMDATA, item, 2);
        item = SendDlgItemMessageA(dialog, 0x3ee, CB_ADDSTRING, 0, (LPARAM)"Nearest");
        SendDlgItemMessageA(dialog, 0x3ee, CB_SETITEMDATA, item, 0);
        item = SendDlgItemMessageA(dialog, 0x3ee, CB_ADDSTRING, 0, (LPARAM)"Linear");
        SendDlgItemMessageA(dialog, 0x3ee, CB_SETITEMDATA, item, 1);
        item = SendDlgItemMessageA(dialog, 0x3ee, CB_ADDSTRING, 0, (LPARAM)"Anisotropic");
        SendDlgItemMessageA(dialog, 0x3ee, CB_SETITEMDATA, item, 2);
        SendDlgItemMessageA(dialog, 0x3ee, CB_SETCURSEL, DAT_0056b8c8.textureFilter, 0);
        item = SendDlgItemMessageA(dialog, 0x3ef, CB_ADDSTRING, 0, (LPARAM)"None");
        SendDlgItemMessageA(dialog, 0x3ef, CB_SETITEMDATA, item, 0);
        item = SendDlgItemMessageA(dialog, 0x3ef, CB_ADDSTRING, 0, (LPARAM)"Nearest");
        SendDlgItemMessageA(dialog, 0x3ef, CB_SETITEMDATA, item, 1);
        item = SendDlgItemMessageA(dialog, 0x3ef, CB_ADDSTRING, 0, (LPARAM)"Linear");
        SendDlgItemMessageA(dialog, 0x3ef, CB_SETITEMDATA, item, 2);
        SendDlgItemMessageA(dialog, 0x3ef, CB_SETCURSEL, DAT_0056b8c8.mipFilter, 0);

        for (int k = 0; k < FUN_00405350(); k++) {
            item = SendDlgItemMessageA(dialog, 0x3f3, CB_ADDSTRING, 0, (LPARAM)FUN_00405330(k));
            SendDlgItemMessageA(dialog, 0x3f3, CB_SETITEMDATA, item, k);
        }
        if (DAT_0056b8c8.language >= FUN_00405350()) {
            DAT_0056b8c8.language = 0;
        }
        SendDlgItemMessageA(dialog, 0x3f3, CB_SELECTSTRING, (WPARAM)-1, (LPARAM)FUN_00405330(DAT_0056b8c8.language));
        if (FUN_00405350() < 2) {
            EnableWindow(GetDlgItem(dialog, 0x3f3), FALSE);
            EnableWindow(GetDlgItem(dialog, 0x40d), FALSE);
        }

        int quality;
        switch (DAT_0056b8c8.textureQuality) {
        case 0:
            quality = 1;
            break;
        case 1:
            quality = 0;
            break;
        default:
            quality = 2;
            break;
        }
        SendDlgItemMessageA(dialog, 0x3ed, CB_SETCURSEL, quality, 0);

        SendMessageA(dialog, WM_SETICON, ICON_BIG, (LPARAM)DAT_0056b98c);
        SendMessageA(dialog, WM_SETICON, ICON_SMALL, (LPARAM)DAT_0056b98c);
        SendDlgItemMessageA(dialog, 0x3fb, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)DAT_0056b964);
        SendDlgItemMessageA(dialog, 0x3fc, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)DAT_0056b960);
        FUN_004a3060(dialog);
        return TRUE;
    } else if (message == WM_COMMAND) {
        if (LOWORD(wParam) == IDOK) {
            DAT_0056b8c8.unk7c = SendDlgItemMessageA(dialog, 0x3eb, BM_GETCHECK, 0, 0) == BST_CHECKED;
            DAT_0056b8c8.unk7d = SendDlgItemMessageA(dialog, 0x3fa, BM_GETCHECK, 0, 0) == BST_CHECKED;
            DAT_0056b8c8.adapter = *DAT_0056b968[DAT_0056b984].adapter->GetGuid();
            memset(DAT_0056b8c8.device, 0, sizeof(DAT_0056b8c8.device));
            strcpy(DAT_0056b8c8.device, DAT_0056b0b0->m_devices[DAT_0056b988].name);
            DAT_0056b8c8.width = DAT_0056b968[DAT_0056b984].mode->width;
            DAT_0056b8c8.height = DAT_0056b968[DAT_0056b984].mode->height;
            DAT_0056b8c8.bpp = DAT_0056b968[DAT_0056b984].mode->bpp;
            EndDialog(dialog, 1);
            return TRUE;
        } else if (LOWORD(wParam) == IDCANCEL) {
            EndDialog(dialog, 2);
            return TRUE;
        } else if (LOWORD(wParam) == 0x3fb) {
            ShellExecuteA(dialog, "open", "iexplore.exe", "http://www.jowood.com/", NULL, SW_SHOWNORMAL);
            return TRUE;
        } else if (LOWORD(wParam) == 0x3fc) {
            ShellExecuteA(dialog, "open", "iexplore.exe", "http://www.bugbear.fi/", NULL, SW_SHOWNORMAL);
            return TRUE;
        } else if (LOWORD(wParam) == 0x3fd) {
            DAT_0056b0f8 = !DAT_0056b0f8;
            EnableWindow(GetDlgItem(dialog, 0x3ed), DAT_0056b0f8);
            EnableWindow(GetDlgItem(dialog, 0x3ef), DAT_0056b0f8);
            EnableWindow(GetDlgItem(dialog, 0x3ee), DAT_0056b0f8);
            EnableWindow(GetDlgItem(dialog, 0x3f4), DAT_0056b0f8);
            EnableWindow(GetDlgItem(dialog, 0x3f5), DAT_0056b0f8);
            EnableWindow(GetDlgItem(dialog, 0x3fe), DAT_0056b0f8);
            EnableWindow(GetDlgItem(dialog, 0x400), DAT_0056b0f8);
            EnableWindow(GetDlgItem(dialog, 0x3ff), DAT_0056b0f8);
            EnableWindow(GetDlgItem(dialog, 0x400), DAT_0056b0f8);
            EnableWindow(GetDlgItem(dialog, 0x401), DAT_0056b0f8);
            EnableWindow(GetDlgItem(dialog, 0x402), DAT_0056b0f8);
            EnableWindow(GetDlgItem(dialog, 0x403), DAT_0056b0f8);
            EnableWindow(GetDlgItem(dialog, 0x409), DAT_0056b0f8);
            EnableWindow(GetDlgItem(dialog, 0x404), DAT_0056b0f8);
            EnableWindow(GetDlgItem(dialog, 0x40b), DAT_0056b0f8);
            EnableWindow(GetDlgItem(dialog, 0x406), DAT_0056b0f8);
        } else if (HIWORD(wParam) == CBN_SELENDOK) {
            int selected = SendMessageA((HWND)lParam, CB_GETCURSEL, 0, 0);
            int data = SendMessageA((HWND)lParam, CB_GETITEMDATA, selected, 0);
            if (selected == CB_ERR) {
                return TRUE;
            }
            switch (LOWORD(wParam)) {
            case 1000:
                if (data != DAT_0056b984) {
                    DAT_0056b984 = data;
                    FUN_004a3060(dialog);
                    return TRUE;
                }
                break;
            case 0x3f2:
                DAT_0056b988 = data;
                return TRUE;
            case 0x3f6:
                DAT_0056b8c8.speakers = data;
                return TRUE;
            case 0x3e9: {
                std::vector<DisplayMode*>* modes = DAT_0056b968[DAT_0056b984].adapter->GetModes();
                DAT_0056b968[DAT_0056b984].modeIndex = data;
                DAT_0056b968[DAT_0056b984].mode = (*modes)[data];
                FUN_004a3170(dialog);
                return TRUE;
            }
            case 0x3f4:
                DAT_0056b968[DAT_0056b984].refresh = data;
                DAT_0056b8c8.refresh = data;
                return TRUE;
            case 0x3f5:
                DAT_0056b968[DAT_0056b984].antialias = data;
                DAT_0056b8c8.antialias = data;
                return TRUE;
            case 0x3ed:
                DAT_0056b8c8.textureQuality = data;
                return TRUE;
            case 0x3ee:
                DAT_0056b8c8.textureFilter = data;
                return TRUE;
            case 0x3ef:
                DAT_0056b8c8.mipFilter = data;
                return TRUE;
            case 0x3f3:
                DAT_0056b8c8.language = data;
                return TRUE;
            case 0x404:
                DAT_0056b8c8.sampleRate = data;
                return TRUE;
            case 0x406:
                DAT_0056b8c8.voices = data;
                break;
            }
            return TRUE;
        }
    }
    return FALSE;
}

// FUNCTION: RALLYTROPHY 0x4a3060
// Fills the display mode combo box with the selected adapter's modes.
void FUN_004a3060(HWND dialog)
{
    SendDlgItemMessageA(dialog, 0x3e9, CB_RESETCONTENT, 0, 0);
    std::vector<DisplayMode*>* modes = DAT_0056b968[DAT_0056b984].adapter->GetModes();
    unsigned int i = 0;
    DisplayMode** first = modes->begin();
    for (; i < modes->size(); i++) {
        DisplayMode* mode = first[i];
        char text[80];
        sprintf(text, "%ld x %ld x %ld", mode->width, mode->height, mode->bpp);
        int item = SendDlgItemMessageA(dialog, 0x3e9, CB_ADDSTRING, 0, (LPARAM)text);
        SendDlgItemMessageA(dialog, 0x3e9, CB_SETITEMDATA, item, i);
        if (mode == DAT_0056b968[DAT_0056b984].mode) {
            SendDlgItemMessageA(dialog, 0x3e9, CB_SETCURSEL, item, 0);
        }
    }
    FUN_004a3170(dialog);
}

// FUNCTION: RALLYTROPHY 0x4a3170
// Fills the refresh rate (0x3f4) and anti-aliasing (0x3f5) combo boxes for the
// selected mode and stores the resulting choices.
void FUN_004a3170(HWND dialog)
{
    SendDlgItemMessageA(dialog, 0x3f4, CB_RESETCONTENT, 0, 0);
    SendDlgItemMessageA(dialog, 0x3f5, CB_RESETCONTENT, 0, 0);
    DisplayMode* mode = DAT_0056b968[DAT_0056b984].mode;
    int refresh = DAT_0056b968[DAT_0056b984].refresh;
    int antialias = DAT_0056b968[DAT_0056b984].antialias;
    int selectedRefresh = 0;
    int selectedAntialias = 0;
    int selected = 0;
    char text[80];
    int item;
    for (int i = 0; i < mode->refreshCount; i++) {
        if (mode->refresh[i] == 0) {
            sprintf(text, "default");
        } else {
            sprintf(text, "%i Hz", mode->refresh[i]);
        }
        item = SendDlgItemMessageA(dialog, 0x3f4, CB_ADDSTRING, 0, (LPARAM)text);
        SendDlgItemMessageA(dialog, 0x3f4, CB_SETITEMDATA, item, mode->refresh[i]);
        if (!selected) {
            SendDlgItemMessageA(dialog, 0x3f4, CB_SETCURSEL, item, 0);
            selected = 1;
        }
        if (refresh == mode->refresh[i]) {
            SendDlgItemMessageA(dialog, 0x3f4, CB_SETCURSEL, item, 0);
            selectedRefresh = mode->refresh[i];
            selected = 1;
        }
    }
    item = SendDlgItemMessageA(dialog, 0x3f5, CB_ADDSTRING, 0, (LPARAM)"None");
    SendDlgItemMessageA(dialog, 0x3f5, CB_SETITEMDATA, item, 0);
    SendDlgItemMessageA(dialog, 0x3f5, CB_SETCURSEL, item, 0);
    for (int samples = 2; samples <= 16; samples++) {
        if (mode->GetAntialias() & (1 << samples)) {
            sprintf(text, "%i samples", samples);
            item = SendDlgItemMessageA(dialog, 0x3f5, CB_ADDSTRING, 0, (LPARAM)text);
            SendDlgItemMessageA(dialog, 0x3f5, CB_SETITEMDATA, item, samples);
            if (antialias == samples) {
                SendDlgItemMessageA(dialog, 0x3f5, CB_SETCURSEL, item, 0);
                selectedAntialias = samples;
            }
        }
    }
    DAT_0056b968[DAT_0056b984].refresh = selectedRefresh;
    DAT_0056b8c8.refresh = selectedRefresh;
    DAT_0056b968[DAT_0056b984].antialias = selectedAntialias;
    DAT_0056b8c8.antialias = selectedAntialias;
}

// FUNCTION: RALLYTROPHY 0x4a3390
// Opens the game window in the selected display mode, configures the renderer,
// loads the shaders and creates the input, sound and game subsystems.
bool FUN_004a3390(HINSTANCE instance)
{
    DAT_0056b0ac = DAT_0056b968[DAT_0056b984].adapter;
    if (DAT_0056b0ac->GetCaps()[1] & 8) {
        DAT_005012e0 = 1;
    }
    HCURSOR cursor = LoadCursorA(NULL, IDC_ARROW);
    int flags = 0;
    if (DAT_0056b8c8.unk7c) {
        flags = 1;
    }
    DAT_0056b0cc = DAT_0056b0ac->OpenWindow(instance, "Rally Trophy", DAT_0056b98c, cursor,
        DAT_0056b968[DAT_0056b984].mode, DAT_0056b8c8.refresh, DAT_0056b8c8.antialias, flags);
    if (!DAT_0056b0cc) {
        DAT_0056b0ac->Release();
        flags &= ~1;
        DAT_0056b8c8.unk7c = 0;
        DAT_0056b0cc = DAT_0056b0ac->OpenWindow(instance, "Rally Trophy", DAT_0056b98c, cursor,
            DAT_0056b968[DAT_0056b984].mode, DAT_0056b8c8.refresh, DAT_0056b8c8.antialias, flags);
        if (!DAT_0056b0cc) {
            DAT_0056b0ac->Release();
            MessageBoxA(NULL, "Cannot open display mode", "Rally Trophy Error", MB_ICONINFORMATION);
            return false;
        }
    }

    DeviceInfo* wanted = &DAT_0056b0b0->m_devices[DAT_0056b988];
    DeviceInfo* device = DAT_0056b0b0->FUN_004989c0(DAT_005015b0[DAT_0056b8c8.sampleRate], 16, 2, 4,
        DAT_00501590[DAT_0056b8c8.voices], wanted, DAT_0056b8c8.speakers);
    if (device != wanted) {
        memset(DAT_0056b8c8.device, 0, sizeof(DAT_0056b8c8.device));
        if (device) {
            strcpy(DAT_0056b8c8.device, device->name);
        }
    }

    DAT_0056b0ac->SetRenderState(6, DAT_0056b8c8.textureQuality);
    DAT_0056b0ac->SetRenderState(7, DAT_0056b8c8.textureFilter);
    DAT_0056b0ac->SetRenderState(8, DAT_0056b8c8.mipFilter);
    int value = DAT_0056b8c8.mipFilter ? 12 : 0;
    DAT_0056b0ac->SetRenderState(9, value);
    DAT_0056aea8 = DAT_0056b0ac->GetWidth() / 1024.0f;
    DAT_0056aeac = DAT_0056b0ac->GetHeight() / 768.0f;
    DAT_0056b0ac->SetRenderState(5, 0);
    DAT_0056b0ac->vf42(8);
    DAT_0056b0ac->vf38(0);

    memset(DAT_0056b0fc, 0, sizeof(DAT_0056b0fc));
    FUN_004a3930("data/shaders/BodyAlphaCube.sha", 1);
    FUN_004a3930("data/shaders/BodyConstCube.sha", 2);
    FUN_004a3930("data/shaders/WindowAlphaCube.sha", 3);
    FUN_004a3930("data/shaders/WindowConstCube.sha", 4);
    FUN_004a3930("data/shaders/OpacityConstCube.sha", 5);
    FUN_004a3930("data/shaders/BodyAlphaStandard.sha", 6);
    FUN_004a3930("data/shaders/BodyConstStandard.sha", 7);
    FUN_004a3930("data/shaders/WindowAlphaStandard.sha", 8);
    FUN_004a3930("data/shaders/WindowConstStandard.sha", 9);
    FUN_004a3930("data/shaders/OpacityConstStandard.sha", 10);
    FUN_004a3930("data/shaders/WindowLodAlphaCube.sha", 11);
    FUN_004a3930("data/shaders/WindowLodConstCube.sha", 12);
    FUN_004a3930("data/shaders/WindowLodAlphaStandard.sha", 13);
    FUN_004a3930("data/shaders/WindowLodConstStandard.sha", 14);
    FUN_004a3930("data/shaders/WaterFall.sha", 15);
    FUN_004a3930("data/shaders/CarShader.sha", 16);
    FUN_004a3930("data/shaders/DamagedWindow.sha", 17);
    FUN_004a3930("data/shaders/Shadow.sha", 18);
    FUN_004a3930("data/shaders/dirt.sha", 19);
    FUN_004a3930("data/shaders/DirtAlphaCube.sha", 20);
    FUN_004a3930("data/shaders/DirtConstCube.sha", 22);
    FUN_004a3930("data/shaders/DirtAlphaStandard.sha", 21);
    FUN_004a3930("data/shaders/DirtConstStandard.sha", 23);
    FUN_004a3930("data/shaders/Lake.sha", 24);

    if ((DAT_0056b0ac->vf76(1) && DAT_0056b0ac->vf76(3) && DAT_0056b0ac->vf76(11)) ||
        (DAT_0056b0ac->vf76(2) && DAT_0056b0ac->vf76(4) && DAT_0056b0ac->vf76(12))) {
        DAT_005012e8 = 1;
    }
    if ((DAT_0056b0ac->vf76(6) && DAT_0056b0ac->vf76(8) && DAT_0056b0ac->vf76(13)) ||
        (DAT_0056b0ac->vf76(7) && DAT_0056b0ac->vf76(9) && DAT_0056b0ac->vf76(14))) {
        DAT_005012ec = 1;
    }

    DAT_0056b0bc = new T_56b0bc();
    DAT_0056b0c0 = new T_56b0c0();
    FUN_004b73b0(1);
    FUN_00405090(DAT_0056b8c8.language);
    DAT_0056b0b4 = new T_56b0b4(DAT_0056b0cc, 1);
    DAT_0056b0b8 = new T_56b0b8();
    DAT_0056b0b8->FUN_0048e660();
    return true;
}

// FUNCTION: RALLYTROPHY 0x4a3930
// Loads a whole file into memory and hands it to the renderer.
void FUN_004a3930(const char* name, int slot)
{
    File file;
    file.Open(name, 0x81);
    void* data = operator new(file.GetSize());
    file.Read(data, file.GetSize());
    DAT_0056b0fc[slot] = data;
    DAT_0056b0ac->LoadFileData(name, data, file.GetSize(), slot);
}

// Readable behavior reference for the exact entry in winmain_cleanup.cpp.
// Shuts down the game's subsystems and frees loaded files.
static void WinMainCleanupBehavior(void)
{
    for (int i = 0; i < 0x40; i++) {
        if (DAT_0056b0fc[i]) {
            operator delete(DAT_0056b0fc[i]);
        }
    }
    if (DAT_0056b0bc) {
        delete DAT_0056b0bc;
    }
    DAT_0056b0bc = 0;
    if (DAT_0056b0c0) {
        delete DAT_0056b0c0;
    }
    DAT_0056b0c0 = 0;
    if (DAT_0056b0b4) {
        delete DAT_0056b0b4;
    }
    DAT_0056b0b4 = 0;
    if (DAT_0056b0b0) {
        delete DAT_0056b0b0;
    }
    DAT_0056b0b0 = 0;
    if (DAT_0056b0ac) {
        DAT_0056b0ac->Release();
        DAT_0056b0ac = 0;
    }
    if (DAT_0056b980) {
        delete DAT_0056b980;
        DAT_0056b980 = 0;
    }
    if (DAT_0056b0b8) {
        delete DAT_0056b0b8;
    }
    DAT_0056b0b8 = 0;
    FUN_004052f0();
    FUN_004a4cb0();
    FUN_004a7da0();
}

// Readable behavior reference for the exact entry in winmain_setup.cpp.
// Returns whether the saved device, adapter and display mode (including refresh
// rate and anti-aliasing) are still available, selecting them if so.
static int ValidateSavedDisplayBehavior(void)
{
    bool found = false;
    std::vector<DeviceInfo>::iterator it = DAT_0056b0b0->m_devices.begin();
    for (unsigned int i = 0; i < DAT_0056b0b0->m_devices.size(); i++, it++) {
        if (!strcmp(it->name, DAT_0056b8c8.device)) {
            found = true;
            DAT_0056b988 = i;
        }
    }
    if (!found) {
        return 0;
    }
    for (int j = 0; j < DAT_0056b968.m_count; j++) {
        if (!memcmp(DAT_0056b968[j].adapter->GetGuid(), &DAT_0056b8c8.adapter, sizeof(GUID))) {
            int width = DAT_0056b8c8.width;
            int height = DAT_0056b8c8.height;
            int bpp = DAT_0056b8c8.bpp;
            int refresh = DAT_0056b8c8.refresh;
            int antialias = DAT_0056b8c8.antialias;
            DAT_0056b984 = j;
            found = false;
            std::vector<DisplayMode*>* modes = DAT_0056b968[j].adapter->GetModes();
            for (unsigned int k = 0; k < modes->size(); k++) {
                DisplayMode* mode = (*modes)[k];
                if (mode->width == width && mode->height == height && mode->bpp == bpp) {
                    for (int r = 0; r < mode->refreshCount; r++) {
                        if (mode->refresh[r] == refresh && (!antialias || (mode->antialias & (1 << antialias)))) {
                            found = true;
                            break;
                        }
                    }
                }
            }
            return found != 0;
        }
    }
    return 0;
}

// FUNCTION: RALLYTROPHY 0x4a3b10
// Plays the Bugbear logo video until it ends or Escape/Enter/Space/a click skips it.
void FUN_004a3b10(void)
{
    BinkVideo* video = new BinkVideo("data/video/bugbear.bik", DAT_0056b0ac, 1);
    int done = 0;
    int first = 1;
    DAT_0056b0ac->SetRenderState(5, 0);
    DAT_0056b0ac->vf42(8);
    while (FUN_004b69f0(0) && !done) {
        if (first) {
            DAT_0056b0bc->ClearKeys();
            DAT_0057ba20 = 0;
            DAT_0057ba24 = 0;
            first = 0;
        }
        if (DAT_0057ba20 || DAT_0057ba28[VK_ESCAPE] || DAT_0057ba28[VK_RETURN] || DAT_0057ba28[VK_SPACE]) {
            done = 1;
        }
        switch (video->Update()) {
        case -1:
            done = 1;
            break;
        case 1:
            if (DAT_0056b0ac->m_active && DAT_0056b0ac->BeginScene()) {
                video->Render(DAT_0056b0ac);
                DAT_0056b0ac->EndScene();
                DAT_0056b0ac->Present();
            }
            break;
        default:
            FUN_004a18d0(500);
            break;
        }
    }
    if (video) {
        delete video;
    }
    DAT_0056b0bc->ClearKeys();
    DAT_0057ba20 = 0;
    DAT_0057ba24 = 0;
}

// FUNCTION: RALLYTROPHY 0x4a3ca0
// Top-level game flow: front-end videos, loading screen, the game, then saves options.
void FUN_004a3ca0(const char* commandLine)
{
    Game* game = new Game();
    if (DAT_0056b0b8->FUN_004853d0()) {
        game->FUN_00434300();
    } else {
        FUN_004a3e40();
        if (!DAT_0057b9f4) {
            FUN_004a3b10();
            if (!DAT_0057b9f4) {
                FUN_004a3fe0();
                if (!DAT_0057b9f4) {
                    LoadingScreen* loading = new LoadingScreen(DAT_0056b0ac, "data/menu/loading0.tga", 0, 1);
                    DAT_0056b0ac->SetRenderState(5, 0);
                    DAT_0056b0ac->vf42(8);
                    DAT_0056b0ac->BeginScene();
                    DAT_0056b0ac->vf38(0);
                    loading->Draw(DAT_0056b0ac, 0);
                    DAT_0056b0ac->EndScene();
                    DAT_0056b0ac->Present();
                    if (loading) {
                        delete loading;
                    }
                    if (!DAT_0057b9f4) {
                        game->FUN_004309d0();
                    }
                }
            }
        }
    }
    if (game) {
        delete game;
    }
    FUN_004a4870("data/options.cfg");
}

// FUNCTION: RALLYTROPHY 0x4a3e40
// Plays the JoWooD logo video (same loop as FUN_004a3b10, plus unused cycle counts).
void FUN_004a3e40(void)
{
    BinkVideo* video = new BinkVideo("data/video/jowood.bik", DAT_0056b0ac, 1);
    int done = 0;
    int first = 1;
    DAT_0056b0ac->SetRenderState(5, 0);
    DAT_0056b0ac->vf42(8);
    while (FUN_004b69f0(0) && !done) {
        if (first) {
            DAT_0056b0bc->ClearKeys();
            DAT_0057ba20 = 0;
            DAT_0057ba24 = 0;
            first = 0;
        }
        if (DAT_0057ba20 || DAT_0057ba28[VK_ESCAPE] || DAT_0057ba28[VK_RETURN] || DAT_0057ba28[VK_SPACE]) {
            done = 1;
        }
        switch (video->Update()) {
        case -1:
            done = 1;
            break;
        case 1:
            Timer::GetCycles();
            if (DAT_0056b0ac->m_active && DAT_0056b0ac->BeginScene()) {
                video->Render(DAT_0056b0ac);
                DAT_0056b0ac->EndScene();
                DAT_0056b0ac->Present();
            }
            Timer::GetCycles();
            break;
        default:
            FUN_004a18d0(500);
            break;
        }
    }
    if (video) {
        delete video;
    }
    DAT_0056b0bc->ClearKeys();
    DAT_0057ba20 = 0;
    DAT_0057ba24 = 0;
}

// FUNCTION: RALLYTROPHY 0x4a3fe0
// Plays the intro video (same loop as FUN_004a3b10).
void FUN_004a3fe0(void)
{
    BinkVideo* video = new BinkVideo("data/video/intro.bik", DAT_0056b0ac, 1);
    int done = 0;
    int first = 1;
    DAT_0056b0ac->SetRenderState(5, 0);
    DAT_0056b0ac->vf42(8);
    while (FUN_004b69f0(0) && !done) {
        if (first) {
            DAT_0056b0bc->ClearKeys();
            DAT_0057ba20 = 0;
            DAT_0057ba24 = 0;
            first = 0;
        }
        if (DAT_0057ba20 || DAT_0057ba28[VK_ESCAPE] || DAT_0057ba28[VK_RETURN] || DAT_0057ba28[VK_SPACE]) {
            done = 1;
        }
        switch (video->Update()) {
        case -1:
            done = 1;
            break;
        case 1:
            if (DAT_0056b0ac->m_active && DAT_0056b0ac->BeginScene()) {
                video->Render(DAT_0056b0ac);
                DAT_0056b0ac->EndScene();
                DAT_0056b0ac->Present();
            }
            break;
        default:
            FUN_004a18d0(500);
            break;
        }
    }
    if (video) {
        delete video;
    }
    DAT_0056b0bc->ClearKeys();
    DAT_0057ba20 = 0;
    DAT_0057ba24 = 0;
}

// FUNCTION: RALLYTROPHY 0x4a4170
// Checks a "XXXX-XXXX-XXXX-XXXX" product key: hex digits from the first three
// groups, modulo 0xffff and xored with a magic value, must equal the last group.
int FUN_004a4170(const char* key)
{
    char buf[20] = "0x";
    __int64 value;
    int checksum;

    strncpy(buf + 2, key + 1, 3);
    strncpy(buf + 5, key + 5, 4);
    strncpy(buf + 9, key + 10, 3);
    buf[12] = 0;
    sscanf(buf, "%I64x", &value);

    strncpy(buf + 2, key + 15, 4);
    buf[6] = 0;
    sscanf(buf, "%x", &checksum);

    return (short)checksum == (short)((value % 0xffff) ^ (short)0x9249);
}

// FUNCTION: RALLYTROPHY 0x4a4250
// Shows the product key dialog until a valid key is entered or the user cancels.
int FUN_004a4250(HINSTANCE instance)
{
    int accepted;
    for (;;) {
        if (DialogBoxParamA(instance, MAKEINTRESOURCE(0x69), NULL, FUN_004a43b0, 0) == IDCANCEL) {
            accepted = 0;
            break;
        }
        if (FUN_004a4170(DAT_0056b1fc)) {
            accepted = 1;
            break;
        }
        MessageBoxA(NULL, DAT_00501cd4, DAT_00501774, MB_ICONHAND | MB_SYSTEMMODAL);
    }
    FUN_004a42e0(accepted);
    return accepted;
}

// FUNCTION: RALLYTROPHY 0x4a42e0
// Stores the typed product key obfuscated in the registry blob; bit 0 of the
// first xor byte records whether the key was accepted.
void FUN_004a42e0(int keyAccepted)
{
    DAT_0056b0d8[0] = rand() & 0xfe;
    DAT_0056b0d8[5] = rand() & 0xfe;
    DAT_0056b0d8[10] = rand() & 0xfe;
    DAT_0056b0d8[15] = rand() & 0xfe;
    if (keyAccepted) {
        DAT_0056b0d8[0] |= 1;
    }
    for (int i = 0; i < 4; i++) {
        DAT_0056b0d8[1 + i] = (DAT_0056b1fc[i] - i * 7 - 0x6e) ^ DAT_0056b0d8[0];
        DAT_0056b0d8[6 + i] = (DAT_0056b1fc[5 + i] ^ DAT_0056b0d8[5]) - i * 13 + 0x5f;
        DAT_0056b0d8[11 + i] = (DAT_0056b1fc[10 + i] - i * 23 - 0x4c) ^ DAT_0056b0d8[10];
        DAT_0056b0d8[16 + i] = (DAT_0056b1fc[15 + i] ^ DAT_0056b0d8[15]) - i * 54 + 0x16;
    }
}

// FUNCTION: RALLYTROPHY 0x4a43b0
// Product key dialog: edit control 0x3f9 holds the key.
BOOL CALLBACK FUN_004a43b0(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_INITDIALOG) {
        SetDlgItemTextA(dialog, 0x3f9, DAT_0056b1fc);
        return TRUE;
    } else if (message == WM_COMMAND) {
        if (LOWORD(wParam) == IDOK) {
            GetDlgItemTextA(dialog, 0x3f9, DAT_0056b1fc, MAX_PATH);
            EndDialog(dialog, 1);
            return TRUE;
        } else if (LOWORD(wParam) == IDCANCEL) {
            EndDialog(dialog, 2);
            return TRUE;
        }
    }
    return FALSE;
}

// FUNCTION: RALLYTROPHY 0x4a4440
// Loads the settings file, falling back to defaults if it's missing or invalid.
void FUN_004a4440(const char* name)
{
    File file;
    bool valid = false;
    if (FUN_004a6050(name)) {
        file.Open(name, 0x81);
        if (file.GetSize() == sizeof(Settings)) {
            file.Read(&DAT_0056b8c8, sizeof(Settings));
            if (DAT_0056b8c8.magic == SETTINGS_MAGIC) {
                valid = true;
            }
        }
    }
    if (!valid) {
        memset(&DAT_0056b8c8, 0, sizeof(Settings));
        DAT_0056b8c8.magic = SETTINGS_MAGIC;
        DAT_0056b8c8.width = 640;
        DAT_0056b8c8.height = 480;
        DAT_0056b8c8.bpp = 16;
        DAT_0056b8c8.unk7c = 1;
        DAT_0056b8c8.textureQuality = 0;
        DAT_0056b8c8.textureFilter = 1;
        DAT_0056b8c8.mipFilter = 1;
        DAT_0056b8c8.antialias = 0;
        DAT_0056b8c8.refresh = 0;
        DAT_0056b8c8.speakers = 0;
        DAT_0056b8c8.unk7d = 1;
        DAT_0056b8c8.unk7e = 0;
        DAT_0056b8c8.sampleRate = 1;
        DAT_0056b8c8.voices = 1;
        DAT_0056b990 = 1;
        int lang = PRIMARYLANGID(GetUserDefaultLangID());
        DAT_0056b8c8.language = 0;
        for (int i = 1; i < FUN_00405350(); i++) {
            if (FUN_004053a0(i) == lang) {
                DAT_0056b8c8.language = i;
                break;
            }
        }
    }
    file.Close();
}

// FUNCTION: RALLYTROPHY 0x4a45c0
// Writes the 0x98-byte settings block to a file.
void FUN_004a45c0(const char* name, int flag)
{
    DAT_0056b8c8.unk7e = flag != 0;
    File file(name, 0x82);
    file.Write(&DAT_0056b8c8, sizeof(DAT_0056b8c8));
    file.Close();
}

// FUNCTION: RALLYTROPHY 0x4a4660
// Loads the progress file (current or previous version) into the option
// blocks, then re-checks the stored product key.
void FUN_004a4660(const char* name)
{
    File file;
    bool valid = false;
    if (FUN_004a6050(name)) {
        file.Open(name, 0x81);
        if (file.GetSize() == sizeof(Progress)) {
            file.Read(&DAT_0056b700, sizeof(Progress));
            if (DAT_0056b700.magic == PROGRESS_MAGIC) {
                valid = true;
            }
        } else if (file.GetSize() == 0x1bc) {
            file.Read(&DAT_0056b700, 0x1bc);
            if (DAT_0056b700.magic == PROGRESS_MAGIC_OLD) {
                DAT_0056b700.unk1bc = 1;
                DAT_0056b700.unk1c0 = 1;
                valid = true;
            }
        }
    }
    if (!valid) {
        FUN_004a42e0(0);
        DAT_0056b994 = 1;
    } else {
        memcpy(DAT_00501300, DAT_0056b700.player, sizeof(DAT_00501300));
        DAT_00501408 = DAT_0056b700.unk108;
        DAT_0050140c = DAT_0056b700.unk10c;
        DAT_00501410 = DAT_0056b700.unk110;
        DAT_00501414 = DAT_0056b700.unk114;
        memcpy(DAT_00501418, DAT_0056b700.detail, sizeof(DAT_00501418));
        memcpy(DAT_00501438, DAT_0056b700.unk138, sizeof(DAT_00501438));
        memcpy(DAT_0056b0d8, DAT_0056b700.key, sizeof(DAT_0056b0d8));
        memcpy(DAT_00501498, DAT_0056b700.unk164, sizeof(DAT_00501498));
        DAT_00501478 = DAT_0056b700.unk1bc;
        DAT_0050147c = DAT_0056b700.unk1c0;
        DAT_00501480 = DAT_0056b700.unk1c4;
        DAT_00501498[18] = 0x55;
        DAT_00501498[19] = 0x15155;
        DAT_00501498[20] = 0x155555;
        DAT_00501498[21] = 0x3fffff;
        if (!FUN_004a4170(FUN_004a1820())) {
            FUN_004a42e0(0);
        }
    }
    DAT_0056b1fc[MAX_PATH - 1] = 0;
    file.Close();
}

// FUNCTION: RALLYTROPHY 0x4a4870
// Saves the option blocks and product key blob to the progress file.
void FUN_004a4870(const char* name)
{
    DAT_0056b700.magic = PROGRESS_MAGIC;
    memcpy(DAT_0056b700.player, DAT_00501300, sizeof(DAT_00501300));
    DAT_0056b700.unk108 = DAT_00501408;
    DAT_0056b700.unk10c = DAT_0050140c;
    DAT_0056b700.unk110 = DAT_00501410;
    DAT_0056b700.unk114 = DAT_00501414;
    memcpy(DAT_0056b700.detail, DAT_00501418, sizeof(DAT_00501418));
    memcpy(DAT_0056b700.unk138, DAT_00501438, sizeof(DAT_00501438));
    memcpy(DAT_0056b700.key, DAT_0056b0d8, sizeof(DAT_0056b0d8));
    memcpy(DAT_0056b700.unk164, DAT_00501498, sizeof(DAT_00501498));
    DAT_0056b700.unk1bc = DAT_00501478;
    DAT_0056b700.unk1c0 = DAT_0050147c;
    DAT_0056b700.unk1c4 = DAT_00501480;
    File file(name, 0x82);
    file.Write(&DAT_0056b700, sizeof(Progress));
    file.Close();
}

// FUNCTION: RALLYTROPHY 0x4a49b0
// Picks a detail level (0-4) from the renderer's capabilities and video memory.
void FUN_004a49b0(void)
{
    if (DAT_0056b0ac) {
        DWORD memory;
        DWORD caps = *DAT_0056b0ac->GetCaps();
        int a = 0;
        int b = 0;
        int level = 0;
        memory = DAT_0056b0ac->m_memory;
        if (caps & 2) a = 1;
        if (caps & 4) b = 1;
        if (a && b && memory > 128) {
            level = 4;
        } else if (a && b && memory > 64) {
            level = 3;
        } else if (a && b) {
            level = 2;
        } else if (memory > 48) {
            level = 2;
        } else if (memory > 40) {
            level = 1;
        }
        DAT_00501418[0] = DAT_005015bc[level];
        DAT_00501418[1] = DAT_005015d0[level];
        DAT_00501418[5] = DAT_005015e4[level];
        DAT_00501418[4] = DAT_005015f8[level];
        DAT_00501418[3] = DAT_0050160c[level];
        DAT_00501418[2] = DAT_00501620[level];
    }
}

// FUNCTION: RALLYTROPHY 0x4a7d30
// SYMBOL: ?FUN_004a7d30@@YGIXZ
// original: FUN_004a7d30
__declspec(naked) unsigned int __stdcall FUN_004a7d30(void)
{
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 8
        fstcw word ptr [ebp - 8]
        mov ecx, dword ptr [ebp - 8]
        test ch, 3
        jnz normalize
        mov al, cl
        and al, 0x3f
        cmp al, 0x3f
        jnz normalize
        test ch, 0xc
        jz saved
normalize:
        mov ax, word ptr [ebp - 8]
        and ax, 0xfcff
        or ax, 0x3f
        and ax, 0xf3ff
        mov word ptr [ebp - 2], ax
        fldcw word ptr [ebp - 2]
saved:
        mov eax, dword ptr [DAT_00577bc4]
        test eax, eax
        jnz done
        mov word ptr [DAT_00577bc0], cx
        mov dword ptr [DAT_00577bc4], 1
done:
        mov ax, cx
        mov esp, ebp
        pop ebp
        ret
    }
}
