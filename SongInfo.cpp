#include<iostream>
#include<dirent.h>
#include<string>
#include<taglib/fileref.h>
#include<stdlib.h>
#include<include_cpp/libxl.h>

libxl::Book* book = xlCreateBookW();
libxl::Font* font = book->addFont();
libxl::Sheet* sheet = book->addSheet(L"SongList");
libxl::Format* format = book->addFormat();
int row = 2;
int songs = 1;

void directoryloop(const char* path)
{
    struct dirent* d;
    char newpath[800];
    DIR* dr = opendir(path);
    if (dr != NULL)
    {
        dr->ent;
        for (d = readdir(dr); d != NULL; d = readdir(dr))
        {
            
            if ((d->d_type == 16384)&&(d->d_name[0]!='.'))
            {
                int i = 0, j = 0;
                
                while (path[i])
                {
                    newpath[i] = path[i];
                    i++;
                }
                newpath[i] = '\\';
                while (d->d_name[j])
                {
                    
                    newpath[i + 1 + j] = d->d_name[j];
                    j++;
                }
                for (int k = i + j + 1; k < 800; k++)
                {
                    newpath[k] = (char)0;
                }
                directoryloop(newpath);
            }
            else if(d->d_name[0] != '.')
            {
                int i = 0, j = 0;
                char songpath[800];
                while (path[i])
                {
                    songpath[i] = path[i];
                    i++;
                }
                songpath[i] = '\\';
                while (d->d_name[j])
                {

                    songpath[i + 1 + j] = d->d_name[j];
                    j++;
                }
                for (int k = i + j + 1; k < 800; k++)
                {
                    songpath[k] = (char)0;
                }
                if ((strstr(songpath,".wav"))||(strstr(songpath, ".flac"))||(strstr(songpath, ".mp3")) || (strstr(songpath, ".aac"))||(strstr(songpath, ".dts"))|| (strstr(songpath, ".mlp")))
                {

                    TagLib::FileRef f(songpath);
                    if (!f.isNull())
                    {
                        if (!f.tag()->album().isEmpty())
                        {
                            std::string albumname = f.tag()->album().toCString();
                            std::wstring wstr = std::wstring(albumname.begin(), albumname.end());
                            const wchar_t* wchar = wstr.c_str();
                            sheet->writeStr(row, 1, wchar, format);
                        }
                        if (!f.tag()->title().isEmpty())
                        {
                            std::string songname = f.tag()->title().toCString();
                            std::wstring swstr = std::wstring(songname.begin(), songname.end());
                            const wchar_t* swchar = swstr.c_str();
                            sheet->writeStr(row, 0, swchar, format);
                        }

                            std::string str = path;
                            std::wstring wstrp = std::wstring(str.begin(), str.end());
                            const wchar_t* pwstr = wstrp.c_str();
                            sheet->writeStr(row, 2, pwstr, format);

                            std::string fname = d->d_name;
                            std::wstring wfname = std::wstring(fname.begin(), fname.end());
                            const wchar_t* wcfname = wfname.c_str();
                            sheet->writeStr(row, 3, wcfname, format);

                            
                            
                            


                            std::cout << "Songs found " << songs << std::endl;
                            row++;
                            songs++;
                    }
               
                }
            }

        }
        closedir(dr);
    }
        
    
}

int main()
{
    font->setSize(10);
    font->setName(L"Arial");
    sheet->writeStr(1, 0, L"Title", format);
    sheet->writeStr(1, 1, L"Album", format);
    sheet->writeStr(1, 2, L"Path", format);
    sheet->writeStr(1, 3, L"File_Name", format);
    char r;
    do
    {
        std::string spath;
        std::cout << "Enter the location:";
        std::cin >> spath;
        const char* path = spath.c_str();
        directoryloop(path);
        std::cout << "\n\nDo you need to add any other Location (y/n):";
        std::cin >> r;
    } while (r == 'y');
    book->save(L"SongInfo.xls");
    book->release();
    std::cout << std::endl << std::endl << "Q to exit:";
    char q;
    std::cin >> q;
    return 0;
}