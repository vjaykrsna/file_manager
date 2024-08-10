#include <filesystem>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;
namespace fs = filesystem;

string path = "/";

void home()
{
    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    int w = ws.ws_col;     // Get terminal width
    int h = ws.ws_row - 4; // Get terminal height

    string text = "FILE MANAGER BY VIJAY KRISHNA 😎.";
    system("clear");
    for (int i = 0; i < h / 2; i++)
    {
        cout << string(w - 1, '`') << endl;
    }

    cout << string((w - text.size()) / 2, '`') + " " + text + string((w - text.size()) / 2, '`') << endl;

    for (int i = 0; i < h / 2; i++)
    {
        cout << string(w - 1, '`') << endl;
    }

    getchar();
    system(("clear"));
}

string fSize(uintmax_t size)
{
    const char *suffixes[] = {"bytes", "KB", "MB", "GB", "TB"};
    int i = 0;
    double doubleSize = static_cast<double>(size);

    while (doubleSize >= 1024 && i < 4)
    {
        doubleSize /= 1024;
        i++;
    }

    return to_string(doubleSize).substr(0, to_string(doubleSize).find(".") + 3) + " " + suffixes[i];
}

void list(const string &path)
{
    cout << left << setw(50) << "Filename" << "Size" << endl;
    cout << string(60, '-') << endl; // Divider

    for (const auto &entry : fs::directory_iterator(path))
    {
        cout << left << setw(50) << entry.path().filename().string();

        if (fs::is_directory(entry.path()))
        {
            cout << "dir";
        }
        else
        {
            cout << fSize(fs::file_size(entry.path()));
        }

        cout << endl;
    }
}

void create(const string &name)
{
    ofstream outFile(name);
    if (!outFile)
    {
        cout << "Failed to create file." << endl;
    }
}

void del(const string &path)
{
    if (!fs::remove(path))
    {
        cout << "File not found" << endl;
    }
}

void renameFile(const string &oldN, const string &newN)
{
    if (fs::exists(oldN))
    {
        fs::rename(oldN, newN);
    }
    else
    {
        cout << "File doesn't exist" << endl;
    }
}

int main(){
    home();
    list(path);
    return 0;
}