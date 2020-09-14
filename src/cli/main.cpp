/*
 * vst-host command line interface
 */
#include <VST.h>
#include <iostream>

int main(int argc, char *argv[])
{
    VSTDatabase db;

    db.AddDirectory("C:\\Program Files (x86)\\VstPlugins");
    db.ScanDirectories();

    auto const &plugins = db.GetPlugins();

    std::cout << "Found " << plugins.size() << " plugins." << std::endl;
    for (auto const &plugin : plugins)
    {
        std::cout << plugin.get()->GetProductName() << std::endl;
    }

    return 0;
}
