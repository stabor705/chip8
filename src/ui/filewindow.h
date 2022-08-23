#ifndef FILEWINDOW_H
#define FILEWINDOW_H

#include <filesystem>
namespace fs = std::filesystem;

class FileWindow {
public:
    void show(float width, float height);
    bool new_file();

    fs::path get_path() const { return filepath; }
    void update(const fs::path &filepath) { this->filepath = filepath; }
private:
    fs::path filepath;
    bool file_loaded {};
};

#endif // FILEWINDOW_H
