/*
 * vst-host command line interface
 */
#include <VST.h>

int main(int argc, char *argv[])
{
    VSTDatabase db;

    db.AddDirectory("C:\\Program Files\\VSTPlugins");
    db.ScanDirectories();

    return 0;
}
