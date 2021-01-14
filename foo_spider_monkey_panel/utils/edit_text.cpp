#include <stdafx.h>

#include "edit_text.h"

#include <fb2k/config.h>
#include <ui/ui_edit_in_progress.h>
#include <ui/ui_editor.h>

#include <qwr/file_helpers.h>
#include <qwr/final_action.h>
#include <qwr/winapi_error_helpers.h>

#include <filesystem>

namespace
{

/// @remark For cases when modal might be called from another modal
class ConditionalModalScope
{
public:
    ConditionalModalScope( HWND hParent )
        : needsModalScope_( modal_dialog_scope::can_create() )
    {
        if ( needsModalScope_ )
        {
            scope_.initialize( hParent );
        }
    }

    ~ConditionalModalScope()
    {
        scope_.deinitialize();
    }

private:
    modal_dialog_scope scope_;
    bool needsModalScope_;
};

/// @throw qwr::QwrException
std::filesystem::path GetFixedEditorPath()
{
    namespace fs = std::filesystem;

    try
    {
        const auto editorPath = fs::u8path( static_cast<const std::u8string&>( smp::config::default_editor ) );
        if ( editorPath.empty() || !fs::exists( editorPath ) )
        {
            smp::config::default_editor = "";
            return fs::path{};
        }
        else
        {
            return editorPath;
        }
    }
    catch ( const fs::filesystem_error& e )
    {
        throw qwr::QwrException( e );
    }
}

} // namespace

namespace smp
{

void EditTextFile( HWND hParent, const std::filesystem::path& file )
{
    const auto editorPath = GetFixedEditorPath();
    if ( editorPath.empty() )
    {
        smp::EditTextFileInternal( hParent, file );
    }
    else
    {
        smp::EditTextFileExternal( editorPath, file );
    }
}

void EditText( HWND hParent, std::u8string& text )
{
    const auto editorPath = GetFixedEditorPath();
    if ( editorPath.empty() )
    {
        smp::EditTextInternal( hParent, text );
    }
    else
    {
        smp::EditTextExternal( hParent, editorPath, text );
    }
}

void EditTextFileInternal( HWND hParent, const std::filesystem::path& file )
{
    // TODO: handle BOM
    auto text = qwr::file::ReadFile( file.u8string(), CP_ACP, true );
    {
        ConditionalModalScope scope( hParent );
        smp::ui::CEditor dlg( file.filename().u8string(), text, [&file, &text] { qwr::file::WriteFile( file.wstring().c_str(), text ); } );
        dlg.DoModal( hParent );
    }
}

void EditTextFileExternal( const std::filesystem::path& pathToEditor, const std::filesystem::path& file )
{
    const std::wstring qPath = L"\"" + file.wstring() + L"\"";
    const auto hInstance = ShellExecute( nullptr,
                                         L"open",
                                         pathToEditor.c_str(),
                                         qPath.c_str(),
                                         nullptr,
                                         SW_SHOW );
    if ( (int)hInstance < 32 )
    { // As per WinAPI
        qwr::error::CheckWin32( (int)hInstance, "ShellExecute" );
    }
}

void EditTextInternal( HWND hParent, std::u8string& text )
{
    ConditionalModalScope scope( hParent );
    smp::ui::CEditor dlg( "Temporary file", text );
    dlg.DoModal( hParent );
}

void EditTextExternal( HWND hParent, const std::filesystem::path& pathToEditor, std::u8string& text )
{
    namespace fs = std::filesystem;

    std::wstring tmpFilePath;
    tmpFilePath.resize( MAX_PATH + 1 );

    DWORD dwRet = GetTempPath( tmpFilePath.size() - 1, tmpFilePath.data() );
    qwr::error::CheckWinApi( dwRet && dwRet <= MAX_PATH, "GetTempPath" );

    std::wstring filename;
    filename.resize( MAX_PATH + 1 );
    UINT uRet = GetTempFileName( tmpFilePath.c_str(),
                                 L"smp",
                                 0,
                                 filename.data() ); // buffer for name
    qwr::error::CheckWinApi( uRet, "GetTempFileName" );

    fs::path fsTmpFilePath( tmpFilePath );
    fsTmpFilePath /= filename;

    qwr::file::WriteFile( fsTmpFilePath, text );
    const qwr::final_action autoRemove( [&fsTmpFilePath] { fs::remove( fsTmpFilePath ); } );

    ConditionalModalScope scope( hParent );
    ui::CEditInProgress dlg{ pathToEditor, fsTmpFilePath };
    if ( dlg.DoModal( hParent ) == IDOK )
    {
        text = qwr::file::ReadFile( fsTmpFilePath.u8string(), CP_UTF8 );
    }
}

} // namespace smp
