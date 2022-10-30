// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <poppler/cpp/poppler-page.h>
#include <poppler/cpp/poppler-document.h>
#include <poppler/cpp/poppler-page-renderer.h>
#include <time.h>
#include <filesystem>
#include <vector>
#include <iterator>
#include <algorithm>

#include <sqlite3.h>

std::filesystem::path thumbnail_path;

bool initDb(sqlite3* db) {
    char* err = nullptr;
    int ret = 0;
    sqlite3_open("Library.db", &db);
    const char* querry = R"(CREATE TABLE IF NOT EXISTS "Books" (
        "Index"	INTEGER NOT NULL UNIQUE,
        "FileName"	TEXT NOT NULL UNIQUE,
        "ImagePath"	TEXT NOT NULL UNIQUE,
        PRIMARY KEY("Index" AUTOINCREMENT)
        ))";

   ret = sqlite3_exec(db, querry, nullptr, nullptr, &err);

   if (ret == SQLITE_OK) {
       std::cout << "Table created successfully\n";
   }
   else {
       std::cout << err << "\n";
       sqlite3_free(err);
       return false;
   }

   return true;
}

std::filesystem::path initThumnailDir() {
    std::filesystem::create_directory("Thumbnails");
    std::filesystem::path pt = std::filesystem::current_path();

    pt /= "Thumbnails";
    return pt;
}


std::filesystem::path generate_thumbnail(std::filesystem::path tb_path, std::filesystem::path book_path, std::filesystem::path book_fileName) {

    std::filesystem::path thumb_nail_path = tb_path;
    thumb_nail_path /= book_fileName;

    poppler::document* doc = nullptr;
    doc = poppler::document::load_from_file(book_path.string());
  
    if (doc != nullptr) {
        std::cout << "Loaded\n";
        poppler::page* pg = doc->create_page(0);

        poppler::page_renderer ren;
        ren.set_image_format(poppler::image::format_argb32);
        poppler::image img = ren.render_page(pg);

        img.save(thumb_nail_path.string() + ".jpeg", "jpeg");

    }

    return thumb_nail_path;

}

void browseDirectory(const char * dir) {
    std::filesystem::path pt = std::filesystem::current_path();

    pt /= "books";

    std::cout << pt.string() << "\n";
    

    if (dir == nullptr) {
        for (auto direct : std::filesystem::directory_iterator(pt)) {
            //std::cout << direct.path().string() << "\n\n";
            //std::cout << direct.path().filename().string() << "\n\n";
            generate_thumbnail(thumbnail_path, direct.path(), direct.path().filename());
       }
    }
    return;
}
void addBookToDb(std::filesystem::path tb_path, std::filesystem::path book_path, std::filesystem::path book_fileName) {

   


}


int main()
{
    
    sqlite3* database = nullptr;
   
    initDb(database);
    
    thumbnail_path = initThumnailDir();

    browseDirectory(nullptr);
    
}

