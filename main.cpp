#include <sys/ioctl.h>
#include <unistd.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
namespace fs = filesystem;

fs::path path = "/";

string fSize(size_t size) {
    const char* suffixes[] = {"bytes", "KB", "MB", "GB", "TB"};
    int i = 0;
    double doubleSize = static_cast<double>(size);

    while (doubleSize >= 1024 && i < 4) {
        doubleSize /= 1024;
        i++;
    }

    return to_string(doubleSize).substr(0, to_string(doubleSize).find(".") + 3) + " " + suffixes[i];
}

void list(const fs::path& path) {
    system("clear");
    cout << left << setw(50) << "Filename"
         << "Size" << endl;
    cout << string(60, '-') << endl;  // Divider

    for (const auto& entry : fs::directory_iterator(path)) {
        cout << left << setw(50) << entry.path().filename().string();

        if (fs::is_directory(entry.path())) {
            cout << "dir";
        } else {
            cout << fSize(fs::file_size(entry.path()));
        }

        cout << endl;
    }
}

void home() {
    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    int w = ws.ws_col;      // Get terminal width
    int h = ws.ws_row - 4;  // Get terminal height

    string text = "FILE MANAGER BY VIJAY KRISHNA 😎.";
    system("clear");
    for (int i = 0; i < h / 2; i++) {
        cout << string(w, '`') << endl;
    }

    cout << string((w - text.size()) / 2, '`') + " " + text + string((w - text.size()) / 2, '`') << endl;

    for (int i = 0; i < h / 2; i++) {
        cout << string(w, '`') << endl;
    }

    cin.ignore();
    cin.get();
    system(("clear"));
    path = "/";
    list(path);
}

void create(const string& name) {
    ofstream outFile(name);
    if (!outFile) {
        cout << "Failed to create file." << endl;
    }
}

void del(const fs::path& path) {
    if (!fs::remove(path)) {
        cout << "File not found" << endl;
    }
}

void moveF(const fs::path& source, const fs::path& destination) {
    if (fs::exists(destination)) {
        cout << "File already exist in destination, do you want to overwrite on cancel?" << endl;
        cout << "'c' for canel, 'o' for overwrite" << endl;
        char ch;
        cin >> ch;
        if (ch == 'c') return;
    } else {
        cout << "File doesn't exists" << endl;
        return;
    }

    fs::rename(source, destination);
}

void renameF(const fs::path& oldN, const fs::path& newN) {
    if (fs::exists(oldN)) {
        fs::rename(oldN, newN);
    } else {
        cout << "File doesn't exist" << endl;
    }
}

void copyF(const fs::path& source, const fs::path& destination) {
    if (fs::exists(destination)) {
        cout << "File already exists in the destination, do you want to overwrite or cancel?" << endl;
        cout << "'c' for cancel, 'o' for overwrite" << endl;
        char ch;
        cin >> ch;
        if (ch == 'c') return;
    }

    fs::copy(source, destination, fs::copy_options::overwrite_existing);
}

void backD(const fs::path& path) {
    if (path.has_parent_path()) {
        ::path = path.parent_path();
        list(::path);
    } else {
        cout << "ALREADY AT ROOT DIRECTORY" << endl;
    }
}

void help() {
    string arr[] = {"1. Create a file: 'create <filename>'",
                    "2. Delete a file/folder: 'delete <filename>'",
                    "3. Rename a file/folder: 'rename <oldname> <newname>'",
                    "4. Copy a file/folder: 'copy <source> <destination>'",
                    "5. Move a file/folder: 'move <source> <destination>'",
                    "6. Go back a directory: 'back'",
                    "7. Move a directory forward: 'chD <dir>'"
                    "",
                    "OTHER COMMANDS:",
                    " - 'home': Go to the home screen.",
                    " - 'list': List all files and folders in the current directory.",
                    " - 'help': Show this help screen.",
                    " - 'exit': Exit the file manager.",
                    "",
                    "HOW TO USE:",
                    "Just type the command followed by the necessary parameters.",
                    "For example, to create a file named 'test.txt', type:",
                    "  create test.txt",
                    "To see the contents of the current directory, type:",
                    "  list"};

    int n = sizeof(arr) / sizeof(arr[0]);
    int mx = -1;
    for (int i = 0; i < n; i++) {
        mx = max(mx, (int)arr[i].size());
    }

    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    int w = ws.ws_col;
    int h = ws.ws_row - 4;

    system("clear");

    for (int i = 0; i < (h - n) / 2; i++) {
        cout << string(w, '`') << endl;
    }

    for (int i = 0; i < n; i++) {
        cout << setw(10) << string(10, '`') << string((w - 20 - mx) / 2, ' ') << setw(mx) << left << arr[i] << string((w - 20 - mx) / 2, ' ') << string(10, '`') << endl;
    }

    for (int i = 0; i < (h - n) / 2; i++) {
        cout << string(w, '`') << endl;
    }

    cin.ignore();
    cin.get();
    system(("clear"));
    list(path);
}

void chD(const fs::path& newPath) {
    if(newPath == path / "~"){
        path = "/";
        list(path);
        return;
    } else if (newPath == path / "..") {
        backD(path);
        return;
    }
    
    if (fs::exists(newPath) && fs::is_directory(newPath)) {
        ::path = newPath;
        list(::path);
    } else {
        cout << "Directory does not exist." << endl;
    }
}

int main() {
    home();

    string command;
    while (true) {
        cout << ">> ";
        cin >> command;

        if (command == "home") {
            home();
        } else if (command == "cd") {
            string dir;
            cin >> dir;
            chD(path / dir);
        } else if (command == "list") {
            list(path);
        } else if (command == "create") {
            string filename;
            cin >> filename;
            create(path / filename);
        } else if (command == "delete") {
            string filename;
            cin >> filename;
            del(path / filename);
        } else if (command == "rename") {
            string oldname, newname;
            cin >> oldname >> newname;
            renameF(path / oldname, path / newname);
        } else if (command == "copy") {
            string source, destination;
            cin >> source >> destination;
            copyF(path / source, path / destination);
        } else if (command == "move") {
            string source, destination;
            cin >> source >> destination;
            moveF(path / source, path / destination);
        } else if (command == "back") {
            backD(::path);
        } else if (command == "help") {
            help();
        } else if (command == "exit") {
            break;
        } else {
            cout << "Unknown command. Type 'help' for a list of commands." << endl;
        }
    }

    return 0;
}