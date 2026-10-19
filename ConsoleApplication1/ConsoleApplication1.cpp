#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <Windows.h>

#pragma execution_character_set("utf-8")

using namespace std;

int vvodChisla(string soobshenie, int minimum, int maximum)
{
    string stroka;

    while (true)
    {
        cout << soobshenie;

        if (!getline(cin, stroka))
            return 0;

        stringstream potok(stroka);
        int chislo;
        char lishniySimvol;

        if (potok >> chislo)
        {
            if (!(potok >> lishniySimvol)
                && chislo >= minimum
                && chislo <= maximum)
            {
                return chislo;
            }
        }

        cout << "Введите целое число от "
            << minimum << " до " << maximum << ".\n";
    }
}

string vvodTexta(string soobshenie)
{
    string text;

    while (true)
    {
        cout << soobshenie;

        if (!getline(cin, text))
            return "";

        if (text.find_first_not_of(" \t\r") != string::npos)
            return text;

        cout << "Поле не должно быть пустым.\n";
    }
}

class Kniga
{
private:
    string nazvanie;
    string avtor;
    int god;
    bool vidana;

public:
    Kniga(string novoeNazvanie, string noviyAvtor, int noviyGod)
    {
        nazvanie = novoeNazvanie;
        avtor = noviyAvtor;
        god = noviyGod;
        vidana = false;
    }

    string getNazvanie() const
    {
        return nazvanie;
    }

    string getAvtor() const
    {
        return avtor;
    }

    int getGod() const
    {
        return god;
    }

    bool getVidana() const
    {
        return vidana;
    }

    void izmenit(string novoeNazvanie, string noviyAvtor, int noviyGod)
    {
        nazvanie = novoeNazvanie;
        avtor = noviyAvtor;
        god = noviyGod;
    }

    bool vidat()
    {
        if (vidana)
            return false;

        vidana = true;
        return true;
    }

    void vernut()
    {
        vidana = false;
    }
};

class Chitatel
{
private:
    string fio;
    int nomerBileta;

    vector<shared_ptr<Kniga>> knigiChitatelya;

public:
    Chitatel(string novoeFio, int noviyNomer)
    {
        fio = novoeFio;
        nomerBileta = noviyNomer;
    }

    string getFio() const
    {
        return fio;
    }

    int getNomerBileta() const
    {
        return nomerBileta;
    }

    int getKolKnig() const
    {
        return (int)knigiChitatelya.size();
    }

    void setFio(string novoeFio)
    {
        fio = novoeFio;
    }

    bool poluchitKnigu(shared_ptr<Kniga> kniga)
    {
        if (kniga->getVidana())
            return false;

        knigiChitatelya.push_back(kniga);
        kniga->vidat();
        return true;
    }

    void pokazKnig() const
    {
        if (knigiChitatelya.empty())
        {
            cout << "У читателя нет книг.\n";
            return;
        }

        for (int i = 0; i < (int)knigiChitatelya.size(); i++)
        {
            cout << i + 1 << ". "
                << knigiChitatelya[i]->getNazvanie()
                << " | " << knigiChitatelya[i]->getAvtor()
                << '\n';
        }
    }

    void vernutKnigu(int indeks)
    {
        if (indeks < 0 || indeks >= (int)knigiChitatelya.size())
            return;

        knigiChitatelya[indeks]->vernut();

        knigiChitatelya.erase(knigiChitatelya.begin() + indeks);
    }
};

class Biblioteka
{
private:
    vector<shared_ptr<Kniga>> knigi;
    vector<unique_ptr<Chitatel>> chitateli;
    int sleduyushiyBilet = 1;

    int viborKnigi()
    {
        pokazKnig();

        if (knigi.empty())
            return -1;

        return vvodChisla(
            "Номер книги (0 - отмена): ",
            0, (int)knigi.size()
        ) - 1;
    }

    int viborChitatelya()
    {
        pokazChitateley();

        if (chitateli.empty())
            return -1;

        return vvodChisla(
            "Номер в списке (0 - отмена): ",
            0, (int)chitateli.size()
        ) - 1;
    }

public:
    void pokazKnig()
    {
        cout << "\nКниги библиотеки\n";

        if (knigi.empty())
        {
            cout << "Книг нет.\n";
            return;
        }

        for (int i = 0; i < (int)knigi.size(); i++)
        {
            cout << i + 1 << ". "
                << knigi[i]->getNazvanie()
                << " | " << knigi[i]->getAvtor()
                << " | " << knigi[i]->getGod()
                << " | ";

            if (knigi[i]->getVidana())
                cout << "Выдана\n";
            else
                cout << "Доступна\n";
        }
    }

    void dobavitKnigu()
    {
        string nazvanie = vvodTexta("Название: ");
        if (!cin) return;

        string avtor = vvodTexta("Автор: ");
        if (!cin) return;

        int god = vvodChisla("Год издания (1-2026): ", 1, 2026);
        if (!cin) return;

        knigi.push_back(make_shared<Kniga>(nazvanie, avtor, god));

        cout << "Книга добавлена.\n";
    }

    void izmenitKnigu()
    {
        int indeks = viborKnigi();
        if (indeks == -1) return;

        string nazvanie = vvodTexta("Новое название: ");
        if (!cin) return;

        string avtor = vvodTexta("Новый автор: ");
        if (!cin) return;

        int god = vvodChisla("Год издания (1-2026): ", 1, 2026);
        if (!cin) return;

        knigi[indeks]->izmenit(nazvanie, avtor, god);

        cout << "Книга изменена.\n";
    }

    void udalitKnigu()
    {
        int indeks = viborKnigi();
        if (indeks == -1) return;

        if (knigi[indeks]->getVidana())
        {
            cout << "Сначала необходимо вернуть книгу.\n";
            return;
        }

        knigi.erase(knigi.begin() + indeks);

        cout << "Книга удалена.\n";
    }

    void pokazChitateley()
    {
        cout << "\nЧитатели библиотеки\n";

        if (chitateli.empty())
        {
            cout << "Читателей нет.\n";
            return;
        }

        for (int i = 0; i < (int)chitateli.size(); i++)
        {
            cout << i + 1 << ". "
                << chitateli[i]->getFio()
                << " | Билет: " << chitateli[i]->getNomerBileta()
                << " | Книг: " << chitateli[i]->getKolKnig()
                << '\n';
        }
    }

    void dobavitChitatelya()
    {
        string fio = vvodTexta("ФИО: ");
        if (!cin) return;

        chitateli.push_back(
            make_unique<Chitatel>(fio, sleduyushiyBilet)
        );

        cout << "Читатель добавлен. Номер билета: "
            << sleduyushiyBilet << '\n';

        sleduyushiyBilet++;
    }

    void izmenitChitatelya()
    {
        int indeks = viborChitatelya();
        if (indeks == -1) return;

        string fio = vvodTexta("Новое ФИО: ");
        if (!cin) return;

        chitateli[indeks]->setFio(fio);

        cout << "Данные изменены. Номер билета сохранён.\n";
    }

    void udalitChitatelya()
    {
        int indeks = viborChitatelya();
        if (indeks == -1) return;

        if (chitateli[indeks]->getKolKnig() > 0)
        {
            cout << "Сначала читатель должен вернуть все книги.\n";
            return;
        }

        chitateli.erase(chitateli.begin() + indeks);

        cout << "Читатель удалён.\n";
    }

    void vidatKnigu()
    {
        int indeksChitatelya = viborChitatelya();
        if (indeksChitatelya == -1) return;

        int indeksKnigi = viborKnigi();
        if (indeksKnigi == -1) return;

        if (chitateli[indeksChitatelya]->poluchitKnigu(knigi[indeksKnigi]))
            cout << "Книга выдана.\n";
        else
            cout << "Книга уже выдана и сейчас недоступна.\n";
    }

    void pokazKnigChitatelya()
    {
        int indeks = viborChitatelya();
        if (indeks == -1) return;

        cout << "\nКниги читателя: "
            << chitateli[indeks]->getFio() << '\n';

        chitateli[indeks]->pokazKnig();
    }

    void vernutKnigu()
    {
        int indeks = viborChitatelya();
        if (indeks == -1) return;

        chitateli[indeks]->pokazKnig();

        if (chitateli[indeks]->getKolKnig() == 0)
            return;

        int nomer = vvodChisla(
            "Номер возвращаемой книги (0 - отмена): ",
            0, chitateli[indeks]->getKolKnig()
        );

        if (nomer == 0) return;

        chitateli[indeks]->vernutKnigu(nomer - 1);

        cout << "Книга возвращена и снова доступна.\n";
    }

    void statistika()
    {
        int vidano = 0;
        int chitateleySKnigami = 0;

        for (int i = 0; i < (int)knigi.size(); i++)
        {
            if (knigi[i]->getVidana())
                vidano++;
        }

        for (int i = 0; i < (int)chitateli.size(); i++)
        {
            if (chitateli[i]->getKolKnig() > 0)
                chitateleySKnigami++;
        }

        cout << "\nСтатистика библиотеки\n";
        cout << "Всего книг: " << knigi.size() << '\n';
        cout << "Выдано книг: " << vidano << '\n';
        cout << "Доступно книг: " << (int)knigi.size() - vidano << '\n';
        cout << "Всего читателей: " << chitateli.size() << '\n';
        cout << "Читателей с книгами: " << chitateleySKnigami << '\n';
    }

    void nachalnieDannie()
    {
        knigi.push_back(
            make_shared<Kniga>("Богатый папа, бедный папа", "Р. Т. Кийосаки", 1997)
        );

        knigi.push_back(
            make_shared<Kniga>("Наследник Империи", "Т. Зан", 1991)
        );

        knigi.push_back(
            make_shared<Kniga>("Каштановый человечек", "С. Свейструп", 2018)
        );

        knigi.push_back(
            make_shared<Kniga>("Дремлющий демон Декстера", "Д. Линдсей", 2004)
        );

        chitateli.push_back(
            make_unique<Chitatel>("Исраэль Йегуда Моргенштерн*", 1)
        );

        chitateli.push_back(
            make_unique<Chitatel>("Алексей Шпак Викторович", 2)
        );

        chitateli.push_back(
            make_unique<Chitatel>("Дарт Вейдер Алексеевич", 3)
        );

        sleduyushiyBilet = 4;
        chitateli[0]->poluchitKnigu(knigi[0]);
        chitateli[1]->poluchitKnigu(knigi[1]);
    }

    void menu()
    {
        int vibor;

        do
        {
            cout << "Библиотека\n";
            cout << "1. Список книг\n";
            cout << "2. Добавить книгу\n";
            cout << "3. Изменить книгу\n";
            cout << "4. Удалить книгу\n";
            cout << "5. Список читателей\n";
            cout << "6. Добавить читателя\n";
            cout << "7. Изменить читателя\n";
            cout << "8. Удалить читателя\n";
            cout << "9. Выдать книгу\n";
            cout << "10. Показать книги читателя\n";
            cout << "11. Вернуть книгу\n";
            cout << "12. Статистика библиотеки\n";
            cout << "0. Выход\n";

            vibor = vvodChisla("Выберите пункт: ", 0, 12);

            switch (vibor)
            {
            case 1:
                pokazKnig();
                break;
            case 2:
                dobavitKnigu();
                break;
            case 3:
                izmenitKnigu();
                break;
            case 4:
                udalitKnigu();
                break;
            case 5:
                pokazChitateley();
                break;
            case 6:
                dobavitChitatelya();
                break;
            case 7:
                izmenitChitatelya();
                break;
            case 8:
                udalitChitatelya();
                break;
            case 9:
                vidatKnigu();
                break;
            case 10:
                pokazKnigChitatelya();
                break;
            case 11:
                vernutKnigu();
                break;
            case 12:
                statistika();
                break;
            case 0:
                cout << "Программа завершена.\n";
                break;
            }

        } while (vibor != 0 && cin);
    }
};

int main()
{
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    Biblioteka* biblioteka = new Biblioteka();

    try
    {
        biblioteka->nachalnieDannie();
        biblioteka->menu();
    }
    catch (...)
    {
        delete biblioteka;
        cout << "Ошибка выполнения программы.\n";
        return 1;
    }
    delete biblioteka;
    biblioteka = nullptr;

    return 0;
}