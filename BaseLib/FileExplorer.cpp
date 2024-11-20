#include "FileExplorer.h"

std::vector<std::string> nims_n::FileExplorer::openFiles(LPCWSTR title, size_t filterCount, const COMDLG_FILTERSPEC* filters)
{
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
        COINIT_DISABLE_OLE1DDE);
    std::vector<std::string> returnFileNames;
    if (SUCCEEDED(hr))
    {
        //std::cout << "started filedialog\n";
        IFileOpenDialog* pFileOpen;
        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&pFileOpen));
        hr = pFileOpen->SetOptions(FOS_FORCEFILESYSTEM | FOS_ALLOWMULTISELECT);
        hr = pFileOpen->SetFileTypes(filterCount, filters);
        hr = pFileOpen->SetTitle(title);

        if (SUCCEEDED(hr))
        {
            // Show the Open dialog box.
            hr = pFileOpen->Show(NULL);
            //hr = pFileOpen->SetDefaultFolder();
            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItemArray* pItem;
                hr = pFileOpen->GetResults(&pItem);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszFilePath = NULL;
                    DWORD fileNumber = 0;
                    hr = pItem->GetCount(&fileNumber);
                    std::wstring selFile;
                    for (DWORD i{ 0 }; i < fileNumber; i++) {
                        IShellItem* psi = NULL;
                        hr = pItem->GetItemAt(i, &psi);
                        if (SUCCEEDED(hr)) {
                            hr = psi->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                            if (SUCCEEDED(hr))
                            {
                                std::wstring selFile;
                                selFile += pszFilePath;
                                std::string res;
                                for (char x : selFile)
                                    res += x;
                                returnFileNames.push_back(res);
                                //	MessageBox(NULL, pszFilePath, L"File Path", MB_OK);
                                CoTaskMemFree(pszFilePath);
                            }
                            psi->Release();

                        }

                    }

                }
            }
            pFileOpen->Release();
        }
        CoUninitialize();
    }

    return returnFileNames;
}

std::string nims_n::FileExplorer::openFile(LPCWSTR title, size_t filterCount, const COMDLG_FILTERSPEC* filters)
{
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
        COINIT_DISABLE_OLE1DDE);
    std::string returnFileName;
    if (SUCCEEDED(hr))
    {
        IFileOpenDialog* pFileOpen;

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&pFileOpen));
        hr = pFileOpen->SetOptions(FOS_FORCEFILESYSTEM);
        hr = pFileOpen->SetFileTypes(filterCount, filters);
        hr = pFileOpen->SetTitle(title);

        if (SUCCEEDED(hr))
        {
            // Show the Open dialog box.
            hr = pFileOpen->Show(NULL);
            //hr = pFileOpen->SetDefaultFolder();
            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItem* pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszFilePath = NULL;
                    DWORD fileNumber = 0;
                    std::wstring selFile;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
                    if (SUCCEEDED(hr)) {
                        std::wstring selFile;
                        selFile += pszFilePath;
                        for (char x : selFile)
                            returnFileName += x;


                        CoTaskMemFree(pszFilePath);

                        pItem->Release();

                    }


                }
            }
            pFileOpen->Release();
        }
        CoUninitialize();
    }
    return returnFileName;
}

std::string nims_n::FileExplorer::openFolder(LPCWSTR title)
{
    std::string returnfileName{""};

    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
        COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        IFileOpenDialog* pFileOpen;

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&pFileOpen));
        hr = pFileOpen->SetOptions(FOS_FORCEFILESYSTEM | FOS_PICKFOLDERS);

        if (SUCCEEDED(hr))
        {
            // Show the Open dialog box.
            hr = pFileOpen->Show(NULL);
            //hr = pFileOpen->SetDefaultFolder();
            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItem* pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszFilePath = NULL;
                    DWORD fileNumber = 0;
                    std::wstring selFile;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
                    if (SUCCEEDED(hr)) {
                        std::wstring selFile;
                        selFile += pszFilePath;
                        for (char x : selFile)
                            returnfileName += x;


                        CoTaskMemFree(pszFilePath);

                        pItem->Release();

                    }


                }
            }
            pFileOpen->Release();
        }
        CoUninitialize();
    }
    return returnfileName;
}
