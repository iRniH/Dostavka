#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include <vector>
using namespace std;

// Структуры данных
struct User {
    int id;
    wstring login;
    wstring password;
    wstring email;
    wstring name;
    wstring phone;
    wstring role;
};

struct Order {
    int id;
    int clientId;
    wstring address;
    wstring date;
    wstring deliveryType;
    wstring comment;
    wstring status;
    int courierId;
    wstring clientName;
};

// Глобальные данные
vector<User> users;
vector<Order> orders;
int nextUserId = 5;
int nextOrderId = 3;
User* currentUser = nullptr;

// Прототипы оконных процедур
LRESULT CALLBACK LoginProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK RegisterProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK AdminProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK OperatorProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ClientProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK CourierProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ProfileProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK EditUserProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK AssignCourierProc(HWND, UINT, WPARAM, LPARAM);

// Глобальные дескрипторы окон
HWND hLoginWnd, hRegisterWnd, hAdminWnd, hOperatorWnd, hClientWnd, hCourierWnd, hProfileWnd;
HWND hEditUserWnd, hAssignCourierWnd;

// Данные для диалогов
User* g_editingUser = nullptr;
Order* g_assigningOrder = nullptr;
int g_selectedCourierId = 0;

// Функция для закрытия всех окон и открытия нужного
void OpenWindow(HWND newWnd) {
    ShowWindow(hLoginWnd, newWnd == hLoginWnd ? SW_SHOW : SW_HIDE);
    ShowWindow(hRegisterWnd, newWnd == hRegisterWnd ? SW_SHOW : SW_HIDE);
    ShowWindow(hAdminWnd, newWnd == hAdminWnd ? SW_SHOW : SW_HIDE);
    ShowWindow(hOperatorWnd, newWnd == hOperatorWnd ? SW_SHOW : SW_HIDE);
    ShowWindow(hClientWnd, newWnd == hClientWnd ? SW_SHOW : SW_HIDE);
    ShowWindow(hCourierWnd, newWnd == hCourierWnd ? SW_SHOW : SW_HIDE);
    ShowWindow(hProfileWnd, newWnd == hProfileWnd ? SW_SHOW : SW_HIDE);
}

// ==================== ОКНО ВХОДА ====================
LRESULT CALLBACK LoginProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        CreateWindow(L"STATIC", L"Логин:", WS_CHILD | WS_VISIBLE, 50, 50, 80, 25, hWnd, NULL, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 140, 50, 150, 25, hWnd, (HMENU)101, NULL, NULL);
        CreateWindow(L"STATIC", L"Пароль:", WS_CHILD | WS_VISIBLE, 50, 100, 80, 25, hWnd, NULL, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 140, 100, 150, 25, hWnd, (HMENU)102, NULL, NULL);
        CreateWindow(L"BUTTON", L"Войти", WS_CHILD | WS_VISIBLE, 100, 160, 100, 35, hWnd, (HMENU)103, NULL, NULL);
        CreateWindow(L"BUTTON", L"Регистрация", WS_CHILD | WS_VISIBLE, 220, 160, 100, 35, hWnd, (HMENU)104, NULL, NULL);
        break;
    }
    case WM_COMMAND: {
        if (LOWORD(wParam) == 103) { // Войти
            wchar_t login[100], pass[100];
            GetWindowText(GetDlgItem(hWnd, 101), login, 100);
            GetWindowText(GetDlgItem(hWnd, 102), pass, 100);

            bool found = false;
            for (auto& u : users) {
                if (u.login == login && u.password == pass) {
                    currentUser = &u;
                    found = true;

                    if (u.role == L"admin") OpenWindow(hAdminWnd);
                    else if (u.role == L"operator") OpenWindow(hOperatorWnd);
                    else if (u.role == L"client") OpenWindow(hClientWnd);
                    else if (u.role == L"courier") OpenWindow(hCourierWnd);
                    break;
                }
            }
            if (!found) {
                MessageBox(hWnd, L"Неверный логин или пароль!\n\nЛогины:\nadmin / 123\noperator / 123\ncourier / 123\nclient / 123",
                    L"Ошибка", MB_OK);
            }
        }
        else if (LOWORD(wParam) == 104) { // Регистрация
            OpenWindow(hRegisterWnd);
        }
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

// ==================== ОКНО РЕГИСТРАЦИИ ====================
LRESULT CALLBACK RegisterProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        CreateWindow(L"STATIC", L"Логин:", WS_CHILD | WS_VISIBLE, 50, 50, 80, 25, hWnd, NULL, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 140, 50, 150, 25, hWnd, (HMENU)201, NULL, NULL);
        CreateWindow(L"STATIC", L"Пароль:", WS_CHILD | WS_VISIBLE, 50, 100, 80, 25, hWnd, NULL, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 140, 100, 150, 25, hWnd, (HMENU)202, NULL, NULL);
        CreateWindow(L"STATIC", L"Email:", WS_CHILD | WS_VISIBLE, 50, 150, 80, 25, hWnd, NULL, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 140, 150, 150, 25, hWnd, (HMENU)203, NULL, NULL);
        CreateWindow(L"BUTTON", L"Зарегистрироваться", WS_CHILD | WS_VISIBLE, 120, 210, 180, 35, hWnd, (HMENU)204, NULL, NULL);
        CreateWindow(L"BUTTON", L"Назад", WS_CHILD | WS_VISIBLE, 50, 210, 60, 35, hWnd, (HMENU)205, NULL, NULL);
        break;
    }
    case WM_COMMAND: {
        if (LOWORD(wParam) == 204) {
            wchar_t login[100], pass[100], email[100];
            GetWindowText(GetDlgItem(hWnd, 201), login, 100);
            GetWindowText(GetDlgItem(hWnd, 202), pass, 100);
            GetWindowText(GetDlgItem(hWnd, 203), email, 100);

            if (wcslen(login) > 0 && wcslen(pass) > 0) {
                users.push_back({ nextUserId++, login, pass, email, login, L"", L"client" });
                MessageBox(hWnd, L"Регистрация успешна! Теперь войдите.", L"Успех", MB_OK);
                OpenWindow(hLoginWnd);
            }
            else {
                MessageBox(hWnd, L"Заполните все поля!", L"Ошибка", MB_OK);
            }
        }
        else if (LOWORD(wParam) == 205) {
            OpenWindow(hLoginWnd);
        }
        break;
    }
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

// ==================== ДИАЛОГ РЕДАКТИРОВАНИЯ ПОЛЬЗОВАТЕЛЯ ====================
HWND hEditName, hEditRole, hEditLogin, hEditPass;

void LoadUserForEdit() {
    if (!g_editingUser) return;
    SetWindowText(hEditName, g_editingUser->name.c_str());
    SetWindowText(hEditRole, g_editingUser->role.c_str());
    SetWindowText(hEditLogin, g_editingUser->login.c_str());
    SetWindowText(hEditPass, g_editingUser->password.c_str());
}

LRESULT CALLBACK EditUserProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        CreateWindow(L"STATIC", L"Имя:", WS_CHILD | WS_VISIBLE, 30, 30, 80, 25, hWnd, NULL, NULL, NULL);
        hEditName = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 120, 30, 200, 25, hWnd, (HMENU)801, NULL, NULL);

        CreateWindow(L"STATIC", L"Роль (admin/operator/client/courier):", WS_CHILD | WS_VISIBLE, 30, 70, 200, 25, hWnd, NULL, NULL, NULL);
        hEditRole = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 240, 70, 150, 25, hWnd, (HMENU)802, NULL, NULL);

        CreateWindow(L"STATIC", L"Логин:", WS_CHILD | WS_VISIBLE, 30, 110, 80, 25, hWnd, NULL, NULL, NULL);
        hEditLogin = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 120, 110, 200, 25, hWnd, (HMENU)803, NULL, NULL);

        CreateWindow(L"STATIC", L"Пароль:", WS_CHILD | WS_VISIBLE, 30, 150, 80, 25, hWnd, NULL, NULL, NULL);
        hEditPass = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 120, 150, 200, 25, hWnd, (HMENU)804, NULL, NULL);

        CreateWindow(L"BUTTON", L"Сохранить", WS_CHILD | WS_VISIBLE, 120, 200, 100, 35, hWnd, (HMENU)805, NULL, NULL);
        CreateWindow(L"BUTTON", L"Отмена", WS_CHILD | WS_VISIBLE, 240, 200, 100, 35, hWnd, (HMENU)806, NULL, NULL);

        LoadUserForEdit();
        break;
    }
    case WM_COMMAND: {
        if (LOWORD(wParam) == 805) { // Сохранить
            wchar_t name[100], role[100], login[100], pass[100];
            GetWindowText(hEditName, name, 100);
            GetWindowText(hEditRole, role, 100);
            GetWindowText(hEditLogin, login, 100);
            GetWindowText(hEditPass, pass, 100);

            g_editingUser->name = name;
            g_editingUser->role = role;
            g_editingUser->login = login;
            g_editingUser->password = pass;

            DestroyWindow(hWnd);
            ShowWindow(hAdminWnd, SW_SHOW);
            // Обновляем список пользователей
            SendMessage(GetDlgItem(hAdminWnd, 301), LB_RESETCONTENT, 0, 0);
            for (auto& u : users) {
                wstring item = to_wstring(u.id) + L" | " + u.name + L" | " + u.role;
                SendMessage(GetDlgItem(hAdminWnd, 301), LB_ADDSTRING, 0, (LPARAM)item.c_str());
            }
            MessageBox(hAdminWnd, L"Пользователь обновлён!", L"Админ", MB_OK);
        }
        else if (LOWORD(wParam) == 806) { // Отмена
            DestroyWindow(hWnd);
            ShowWindow(hAdminWnd, SW_SHOW);
        }
        break;
    }
    case WM_DESTROY:
        hEditUserWnd = NULL;
        break;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

// ==================== ОКНО АДМИНИСТРАТОРА ====================
HWND hUserList;

void UpdateUserList() {
    SendMessage(hUserList, LB_RESETCONTENT, 0, 0);
    for (auto& u : users) {
        wstring item = to_wstring(u.id) + L" | " + u.name + L" | " + u.role;
        SendMessage(hUserList, LB_ADDSTRING, 0, (LPARAM)item.c_str());
    }
}

LRESULT CALLBACK AdminProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        CreateWindow(L"STATIC", L"Управление пользователями:", WS_CHILD | WS_VISIBLE, 20, 20, 200, 25, hWnd, NULL, NULL, NULL);
        hUserList = CreateWindow(L"LISTBOX", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 20, 50, 400, 250, hWnd, (HMENU)301, NULL, NULL);
        CreateWindow(L"BUTTON", L"Добавить", WS_CHILD | WS_VISIBLE, 450, 60, 120, 35, hWnd, (HMENU)302, NULL, NULL);
        CreateWindow(L"BUTTON", L"Удалить", WS_CHILD | WS_VISIBLE, 450, 120, 120, 35, hWnd, (HMENU)303, NULL, NULL);
        CreateWindow(L"BUTTON", L"Редактировать", WS_CHILD | WS_VISIBLE, 450, 180, 120, 35, hWnd, (HMENU)305, NULL, NULL);
        CreateWindow(L"BUTTON", L"Выйти", WS_CHILD | WS_VISIBLE, 20, 350, 100, 35, hWnd, (HMENU)304, NULL, NULL);
        UpdateUserList();
        break;
    }
    case WM_COMMAND: {
        if (LOWORD(wParam) == 302) { // Добавить
            users.push_back({ nextUserId++, L"newuser" + to_wstring(nextUserId), L"123", L"new@mail.ru", L"Новый пользователь", L"", L"client" });
            UpdateUserList();
            MessageBox(hWnd, L"Пользователь добавлен!", L"Админ", MB_OK);
        }
        else if (LOWORD(wParam) == 303) { // Удалить
            int sel = SendMessage(hUserList, LB_GETCURSEL, 0, 0);
            if (sel != LB_ERR && sel < (int)users.size()) {
                if (users[sel].role == L"admin") {
                    MessageBox(hWnd, L"Нельзя удалить администратора!", L"Ошибка", MB_OK);
                }
                else {
                    users.erase(users.begin() + sel);
                    UpdateUserList();
                    MessageBox(hWnd, L"Пользователь удалён!", L"Админ", MB_OK);
                }
            }
        }
        else if (LOWORD(wParam) == 305) { // Редактировать
            int sel = SendMessage(hUserList, LB_GETCURSEL, 0, 0);
            if (sel != LB_ERR && sel < (int)users.size()) {
                g_editingUser = &users[sel];
                ShowWindow(hWnd, SW_HIDE);
                hEditUserWnd = CreateWindow(L"EditUserClass", L"Редактирование пользователя",
                    WS_OVERLAPPEDWINDOW | WS_VISIBLE, 200, 200, 450, 300, NULL, NULL, GetModuleHandle(NULL), NULL);
            }
            else {
                MessageBox(hWnd, L"Выберите пользователя для редактирования!", L"Ошибка", MB_OK);
            }
        }
        else if (LOWORD(wParam) == 304) { // Выйти
            currentUser = nullptr;
            OpenWindow(hLoginWnd);
        }
        break;
    }
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

// ==================== ДИАЛОГ НАЗНАЧЕНИЯ КУРЬЕРА ====================
HWND hCourierCombo;

void FillCourierCombo() {
    SendMessage(hCourierCombo, CB_RESETCONTENT, 0, 0);
    for (auto& u : users) {
        if (u.role == L"courier") {
            wstring item = to_wstring(u.id) + L" - " + u.name;
            SendMessage(hCourierCombo, CB_ADDSTRING, 0, (LPARAM)item.c_str());
        }
    }
    SendMessage(hCourierCombo, CB_SETCURSEL, 0, 0);
}

LRESULT CALLBACK AssignCourierProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        CreateWindow(L"STATIC", L"Выберите курьера:", WS_CHILD | WS_VISIBLE, 30, 30, 150, 25, hWnd, NULL, NULL, NULL);
        hCourierCombo = CreateWindow(L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | CBS_DROPDOWNLIST,
            180, 30, 200, 100, hWnd, (HMENU)901, NULL, NULL);
        CreateWindow(L"BUTTON", L"Назначить", WS_CHILD | WS_VISIBLE, 120, 80, 100, 35, hWnd, (HMENU)902, NULL, NULL);
        CreateWindow(L"BUTTON", L"Отмена", WS_CHILD | WS_VISIBLE, 240, 80, 100, 35, hWnd, (HMENU)903, NULL, NULL);
        FillCourierCombo();
        break;
    }
    case WM_COMMAND: {
        if (LOWORD(wParam) == 902) { // Назначить
            int sel = SendMessage(hCourierCombo, CB_GETCURSEL, 0, 0);
            if (sel != CB_ERR) {
                wchar_t selected[100];
                SendMessage(hCourierCombo, CB_GETLBTEXT, sel, (LPARAM)selected);
                // Извлекаем ID курьера
                int courierId = _wtoi(selected);
                if (g_assigningOrder) {
                    g_assigningOrder->courierId = courierId;
                    g_assigningOrder->status = L"assigned";

                    // Обновляем список заказов у оператора
                    SendMessage(GetDlgItem(hOperatorWnd, 401), LB_RESETCONTENT, 0, 0);
                    for (auto& o : orders) {
                        wstring item = to_wstring(o.id) + L" | " + o.status + L" | " + o.clientName + L" | " + o.date;
                        SendMessage(GetDlgItem(hOperatorWnd, 401), LB_ADDSTRING, 0, (LPARAM)item.c_str());
                    }

                    MessageBox(hWnd, L"Курьер назначен!", L"Оператор", MB_OK);
                }
            }
            DestroyWindow(hWnd);
            ShowWindow(hOperatorWnd, SW_SHOW);
        }
        else if (LOWORD(wParam) == 903) { // Отмена
            DestroyWindow(hWnd);
            ShowWindow(hOperatorWnd, SW_SHOW);
        }
        break;
    }
    case WM_DESTROY:
        hAssignCourierWnd = NULL;
        break;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

// ==================== ОКНО ОПЕРАТОРА ====================
HWND hOrderList;

void UpdateOrderList() {
    SendMessage(hOrderList, LB_RESETCONTENT, 0, 0);
    for (auto& o : orders) {
        wstring item = to_wstring(o.id) + L" | " + o.status + L" | " + o.clientName + L" | " + o.date;
        SendMessage(hOrderList, LB_ADDSTRING, 0, (LPARAM)item.c_str());
    }
}

LRESULT CALLBACK OperatorProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        CreateWindow(L"STATIC", L"Список заказов:", WS_CHILD | WS_VISIBLE, 20, 20, 150, 25, hWnd, NULL, NULL, NULL);
        hOrderList = CreateWindow(L"LISTBOX", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 20, 50, 500, 250, hWnd, (HMENU)401, NULL, NULL);
        CreateWindow(L"BUTTON", L"Назначить курьера", WS_CHILD | WS_VISIBLE, 30, 320, 150, 35, hWnd, (HMENU)402, NULL, NULL);
        CreateWindow(L"BUTTON", L"Изменить статус", WS_CHILD | WS_VISIBLE, 200, 320, 150, 35, hWnd, (HMENU)403, NULL, NULL);
        CreateWindow(L"BUTTON", L"Выйти", WS_CHILD | WS_VISIBLE, 20, 400, 100, 35, hWnd, (HMENU)404, NULL, NULL);

        // Список курьеров для информации
        CreateWindow(L"STATIC", L"Доступные курьеры:", WS_CHILD | WS_VISIBLE, 550, 20, 150, 25, hWnd, NULL, NULL, NULL);
        HWND hCourierInfo = CreateWindow(L"LISTBOX", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 550, 50, 200, 150, hWnd, (HMENU)405, NULL, NULL);

        // Заполняем список курьеров
        for (auto& u : users) {
            if (u.role == L"courier") {
                wstring item = L"ID:" + to_wstring(u.id) + L" | " + u.name;
                SendMessage(hCourierInfo, LB_ADDSTRING, 0, (LPARAM)item.c_str());
            }
        }

        UpdateOrderList();
        break;
    }
    case WM_COMMAND: {
        if (LOWORD(wParam) == 402) { // Назначить курьера
            int sel = SendMessage(hOrderList, LB_GETCURSEL, 0, 0);
            if (sel != LB_ERR && sel < (int)orders.size()) {
                g_assigningOrder = &orders[sel];
                ShowWindow(hWnd, SW_HIDE);
                hAssignCourierWnd = CreateWindow(L"AssignCourierClass", L"Назначение курьера",
                    WS_OVERLAPPEDWINDOW | WS_VISIBLE, 200, 200, 450, 180, NULL, NULL, GetModuleHandle(NULL), NULL);
            }
            else {
                MessageBox(hWnd, L"Выберите заказ для назначения курьера!", L"Ошибка", MB_OK);
            }
        }
        else if (LOWORD(wParam) == 403) { // Изменить статус
            int sel = SendMessage(hOrderList, LB_GETCURSEL, 0, 0);
            if (sel != LB_ERR && sel < (int)orders.size()) {
                if (orders[sel].status == L"new") orders[sel].status = L"assigned";
                else if (orders[sel].status == L"assigned") orders[sel].status = L"delivered";
                else orders[sel].status = L"new";
                UpdateOrderList();
                MessageBox(hWnd, L"Статус изменён!", L"Оператор", MB_OK);
            }
            else {
                MessageBox(hWnd, L"Выберите заказ!", L"Ошибка", MB_OK);
            }
        }
        else if (LOWORD(wParam) == 404) { // Выйти
            currentUser = nullptr;
            OpenWindow(hLoginWnd);
        }
        break;
    }
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

// ==================== ОКНО КЛИЕНТА ====================
HWND hClientAddrEdit, hClientDateEdit, hClientTypeEdit, hClientCommentEdit, hClientOrderList;

void UpdateClientOrderList() {
    if (!currentUser) return;
    SendMessage(hClientOrderList, LB_RESETCONTENT, 0, 0);
    for (auto& o : orders) {
        if (o.clientId == currentUser->id) {
            wstring item = L"#" + to_wstring(o.id) + L" | " + o.status + L" | " + o.address;
            SendMessage(hClientOrderList, LB_ADDSTRING, 0, (LPARAM)item.c_str());
        }
    }
}

LRESULT CALLBACK ClientProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        CreateWindow(L"BUTTON", L"Профиль", WS_CHILD | WS_VISIBLE, 20, 20, 100, 30, hWnd, (HMENU)501, NULL, NULL);

        CreateWindow(L"STATIC", L"Адрес доставки:", WS_CHILD | WS_VISIBLE, 20, 70, 120, 25, hWnd, NULL, NULL, NULL);
        hClientAddrEdit = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 150, 70, 350, 25, hWnd, (HMENU)502, NULL, NULL);

        CreateWindow(L"STATIC", L"Дата (ГГГГ-ММ-ДД):", WS_CHILD | WS_VISIBLE, 20, 110, 130, 25, hWnd, NULL, NULL, NULL);
        hClientDateEdit = CreateWindow(L"EDIT", L"2026-12-31", WS_CHILD | WS_VISIBLE | WS_BORDER, 160, 110, 150, 25, hWnd, (HMENU)503, NULL, NULL);

        CreateWindow(L"STATIC", L"Тип доставки:", WS_CHILD | WS_VISIBLE, 20, 150, 120, 25, hWnd, NULL, NULL, NULL);
        hClientTypeEdit = CreateWindow(L"EDIT", L"Стандарт", WS_CHILD | WS_VISIBLE | WS_BORDER, 150, 150, 150, 25, hWnd, (HMENU)504, NULL, NULL);

        CreateWindow(L"STATIC", L"Комментарий:", WS_CHILD | WS_VISIBLE, 20, 190, 120, 25, hWnd, NULL, NULL, NULL);
        hClientCommentEdit = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE, 150, 190, 350, 60, hWnd, (HMENU)505, NULL, NULL);

        CreateWindow(L"BUTTON", L"Создать заказ", WS_CHILD | WS_VISIBLE, 150, 270, 140, 35, hWnd, (HMENU)506, NULL, NULL);

        CreateWindow(L"STATIC", L"Мои заказы:", WS_CHILD | WS_VISIBLE, 20, 330, 120, 25, hWnd, NULL, NULL, NULL);
        hClientOrderList = CreateWindow(L"LISTBOX", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 20, 360, 500, 150, hWnd, (HMENU)507, NULL, NULL);

        CreateWindow(L"BUTTON", L"Выйти", WS_CHILD | WS_VISIBLE, 20, 540, 100, 35, hWnd, (HMENU)508, NULL, NULL);

        UpdateClientOrderList();
        break;
    }
    case WM_COMMAND: {
        if (LOWORD(wParam) == 501) { // Профиль
            OpenWindow(hProfileWnd);
        }
        else if (LOWORD(wParam) == 506) { // Создать заказ
            wchar_t addr[256], date[100], type[100], comment[500];
            GetWindowText(hClientAddrEdit, addr, 256);
            GetWindowText(hClientDateEdit, date, 100);
            GetWindowText(hClientTypeEdit, type, 100);
            GetWindowText(hClientCommentEdit, comment, 500);

            if (wcslen(addr) > 0) {
                orders.push_back({ nextOrderId++, currentUser->id, addr, date, type, comment, L"new", 0, currentUser->name });
                UpdateClientOrderList();
                UpdateOrderList();
                MessageBox(hWnd, L"Заказ создан успешно!", L"Клиент", MB_OK);
                SetWindowText(hClientAddrEdit, L"");
                SetWindowText(hClientCommentEdit, L"");
            }
            else {
                MessageBox(hWnd, L"Введите адрес доставки!", L"Ошибка", MB_OK);
            }
        }
        else if (LOWORD(wParam) == 508) { // Выйти
            currentUser = nullptr;
            OpenWindow(hLoginWnd);
        }
        break;
    }
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

// ==================== ОКНО КУРЬЕРА ====================
HWND hCourierTaskList;

void UpdateCourierTaskList() {
    if (!currentUser) return;
    SendMessage(hCourierTaskList, LB_RESETCONTENT, 0, 0);
    int taskCount = 0;
    for (auto& o : orders) {
        if (o.courierId == currentUser->id && o.status != L"delivered") {
            wstring item = o.address + L" | " + o.date + L" | " + o.status;
            SendMessage(hCourierTaskList, LB_ADDSTRING, 0, (LPARAM)item.c_str());
            taskCount++;
        }
    }
    if (taskCount == 0) {
        SendMessage(hCourierTaskList, LB_ADDSTRING, 0, (LPARAM)L"Нет активных заданий");
    }
}

LRESULT CALLBACK CourierProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        CreateWindow(L"STATIC", L"Мои задания:", WS_CHILD | WS_VISIBLE, 20, 20, 150, 25, hWnd, NULL, NULL, NULL);
        hCourierTaskList = CreateWindow(L"LISTBOX", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 20, 50, 500, 200, hWnd, (HMENU)601, NULL, NULL);
        CreateWindow(L"BUTTON", L"Подтвердить доставку", WS_CHILD | WS_VISIBLE, 30, 280, 180, 35, hWnd, (HMENU)602, NULL, NULL);
        CreateWindow(L"BUTTON", L"Выйти", WS_CHILD | WS_VISIBLE, 20, 350, 100, 35, hWnd, (HMENU)603, NULL, NULL);
        UpdateCourierTaskList();
        break;
    }
    case WM_COMMAND: {
        if (LOWORD(wParam) == 602) { // Подтвердить доставку
            bool confirmed = false;
            for (auto& o : orders) {
                if (o.courierId == currentUser->id && o.status == L"assigned") {
                    o.status = L"delivered";
                    confirmed = true;
                    break;
                }
            }
            if (confirmed) {
                UpdateCourierTaskList();
                UpdateOrderList();
                MessageBox(hWnd, L"Доставка подтверждена! Заказ завершён.", L"Курьер", MB_OK);
            }
            else {
                MessageBox(hWnd, L"Нет заданий для подтверждения!", L"Ошибка", MB_OK);
            }
        }
        else if (LOWORD(wParam) == 603) { // Выйти
            currentUser = nullptr;
            OpenWindow(hLoginWnd);
        }
        break;
    }
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

// ==================== ОКНО ПРОФИЛЯ ====================
HWND hProfileName, hProfilePhone, hProfileEmail;

void LoadProfileData() {
    if (!currentUser) return;
    SetWindowText(hProfileName, currentUser->name.c_str());
    SetWindowText(hProfilePhone, currentUser->phone.c_str());
    SetWindowText(hProfileEmail, currentUser->email.c_str());
}

void SaveProfileData() {
    if (!currentUser) return;
    wchar_t name[256], phone[256], email[256];
    GetWindowText(hProfileName, name, 256);
    GetWindowText(hProfilePhone, phone, 256);
    GetWindowText(hProfileEmail, email, 256);
    currentUser->name = name;
    currentUser->phone = phone;
    currentUser->email = email;
}

LRESULT CALLBACK ProfileProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        CreateWindow(L"STATIC", L"Имя:", WS_CHILD | WS_VISIBLE, 50, 50, 80, 25, hWnd, NULL, NULL, NULL);
        hProfileName = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 140, 50, 250, 25, hWnd, (HMENU)701, NULL, NULL);

        CreateWindow(L"STATIC", L"Телефон:", WS_CHILD | WS_VISIBLE, 50, 100, 80, 25, hWnd, NULL, NULL, NULL);
        hProfilePhone = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 140, 100, 250, 25, hWnd, (HMENU)702, NULL, NULL);

        CreateWindow(L"STATIC", L"Email:", WS_CHILD | WS_VISIBLE, 50, 150, 80, 25, hWnd, NULL, NULL, NULL);
        hProfileEmail = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 140, 150, 250, 25, hWnd, (HMENU)703, NULL, NULL);

        CreateWindow(L"BUTTON", L"Сохранить", WS_CHILD | WS_VISIBLE, 140, 210, 120, 35, hWnd, (HMENU)704, NULL, NULL);
        CreateWindow(L"BUTTON", L"Назад", WS_CHILD | WS_VISIBLE, 50, 210, 80, 35, hWnd, (HMENU)705, NULL, NULL);

        LoadProfileData();
        break;
    }
    case WM_COMMAND: {
        if (LOWORD(wParam) == 704) { // Сохранить
            SaveProfileData();
            MessageBox(hWnd, L"Профиль сохранён!", L"Успех", MB_OK);
            OpenWindow(hClientWnd);
        }
        else if (LOWORD(wParam) == 705) { // Назад
            OpenWindow(hClientWnd);
        }
        break;
    }
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

// ==================== ТОЧКА ВХОДА ====================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Регистрируем классы окон
    WNDCLASS wc = {};
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    wc.lpfnWndProc = LoginProc;
    wc.lpszClassName = L"LoginClass";
    RegisterClass(&wc);

    wc.lpfnWndProc = RegisterProc;
    wc.lpszClassName = L"RegisterClass";
    RegisterClass(&wc);

    wc.lpfnWndProc = AdminProc;
    wc.lpszClassName = L"AdminClass";
    RegisterClass(&wc);

    wc.lpfnWndProc = OperatorProc;
    wc.lpszClassName = L"OperatorClass";
    RegisterClass(&wc);

    wc.lpfnWndProc = ClientProc;
    wc.lpszClassName = L"ClientClass";
    RegisterClass(&wc);

    wc.lpfnWndProc = CourierProc;
    wc.lpszClassName = L"CourierClass";
    RegisterClass(&wc);

    wc.lpfnWndProc = ProfileProc;
    wc.lpszClassName = L"ProfileClass";
    RegisterClass(&wc);

    wc.lpfnWndProc = EditUserProc;
    wc.lpszClassName = L"EditUserClass";
    RegisterClass(&wc);

    wc.lpfnWndProc = AssignCourierProc;
    wc.lpszClassName = L"AssignCourierClass";
    RegisterClass(&wc);

    // Создаём окна (скрытыми)
    hLoginWnd = CreateWindow(L"LoginClass", L"Авторизация - Система доставки",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 450, 300, NULL, NULL, hInstance, NULL);
    hRegisterWnd = CreateWindow(L"RegisterClass", L"Регистрация - Система доставки",
        WS_OVERLAPPEDWINDOW, 100, 100, 450, 350, NULL, NULL, hInstance, NULL);
    hAdminWnd = CreateWindow(L"AdminClass", L"Панель администратора",
        WS_OVERLAPPEDWINDOW, 100, 100, 650, 450, NULL, NULL, hInstance, NULL);
    hOperatorWnd = CreateWindow(L"OperatorClass", L"Панель оператора",
        WS_OVERLAPPEDWINDOW, 100, 100, 800, 500, NULL, NULL, hInstance, NULL);
    hClientWnd = CreateWindow(L"ClientClass", L"Кабинет клиента",
        WS_OVERLAPPEDWINDOW, 100, 100, 600, 650, NULL, NULL, hInstance, NULL);
    hCourierWnd = CreateWindow(L"CourierClass", L"Панель курьера",
        WS_OVERLAPPEDWINDOW, 100, 100, 600, 450, NULL, NULL, hInstance, NULL);
    hProfileWnd = CreateWindow(L"ProfileClass", L"Профиль пользователя",
        WS_OVERLAPPEDWINDOW, 100, 100, 500, 350, NULL, NULL, hInstance, NULL);

    // Заполняем тестовые данные (добавили больше курьеров)
    users.push_back({ 1, L"admin", L"123", L"admin@mail.ru", L"Администратор", L"+79990001122", L"admin" });
    users.push_back({ 2, L"operator", L"123", L"op@mail.ru", L"Иван Оператор", L"+79991112233", L"operator" });
    users.push_back({ 3, L"courier1", L"123", L"kur1@mail.ru", L"Петр Курьер", L"+79992223344", L"courier" });
    users.push_back({ 4, L"client", L"123", L"client@mail.ru", L"Анна Клиент", L"+79993334455", L"client" });
    users.push_back({ 5, L"courier2", L"123", L"kur2@mail.ru", L"Сергей Курьер", L"+79994445566", L"courier" });
    users.push_back({ 6, L"courier3", L"123", L"kur3@mail.ru", L"Дмитрий Курьер", L"+79995556677", L"courier" });
    nextUserId = 7;

    orders.push_back({ 1, 4, L"ул. Ленина 10", L"2026-05-15", L"Стандарт", L"Позвонить за час", L"new", 0, L"Анна Клиент" });
    orders.push_back({ 2, 4, L"пр. Мира 25", L"2026-05-16", L"Экспресс", L"Домофон 123", L"assigned", 3, L"Анна Клиент" });
    nextOrderId = 3;

    // Показываем только окно входа
    OpenWindow(hLoginWnd);

    // Цикл сообщений
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}