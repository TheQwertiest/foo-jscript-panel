# Current
- Backport all the .js changes from foo-jscript-panel.
- Complete rebranding to spider_monkey_panel (including folders).
- Check and fix all the basic samples.
- Fix unresponsive arrow keys in DUI in CaTRoX: https://github.com/TheQwertiest/foo-jscript-panel/commit/8a41c524a9c3c16c466204fde81c5c108975310b#commitcomment-29727611.

# Prerelease-Test
- Check that EstimateLineWrap results are identical with foo-jscript-panel.
- Test `include` with various codepages (esp. JP,KR,CH).
- Test methods that uses path with arguments containing cyrillics and other exotic characters.
- Check utf8 literals everywhere (paths, script window, script files, properties, callback arguments)
- Check drag-n-drop functionality.
- Check that all files are correctly parsed: UTF8, UTF8-BOM, ASCII.
- Check that it can work side-by-side with foo-jscript-panel.
- Check that stackblur results are identical with foo-jscript-panel.
- Check that kmeans results are identical with foo-jscript-panel.
- Profile CPU usage and check if there are any bottlenecks (note: pfc utf8<>wide is much slower than WinAPI WideCharToMultiByte equivalent)
- Make sure there are no memory leaks.

# Prerelease-Doc
- Add `API changes` page or file with detailed description of API changes and reasons behind them.
- Document new features: ES6, ES2016, ES2017 (with links to feature breakdown), possible ESNext in the future, performance, new methods.
- Document that Dispose() and toArray() methods are not needed anymore.
- Document FbHandleList.Item removal ([] proxy)
- Document that everything is case-sensitive (unlike how it was in WSH\IE...)
- Document discontinued support for WinXP and Vista (i.e. only Win7 and above).
- Document global.include.
- Document DefinePanel.
- Document CreateHtmlWindow (7 args in callback max, arrays are not supported, use JSON).
- Document `Advanced` settings.
- Copy and update marc2003's wiki.
- Create a simple JSP > SMP script migration guide.
- Create a building guide for devs.
- Update docs.
- Check all TODOs.

# Planned
- Replace unmanaged HANDLE (HFONT, HDC and etc) with smart pointers.
- BEWARE: foo_ui_hacks leaks a HFONT every time "SendMessage( hTooltipWnd, WM_SETFONT, (WPARAM)hFont, MAKELPARAM( FALSE, 0 ) )" is called.
- Add option for global.include to skip file if it was loaded before, like include guard (enabled by default).
- Extract common options handling code (i.e. options = {opt1: val1, opt2: val2}; ).
- Consider replacing std::optional with exceptions: need to be very careful - exceptions MUST NOT leak to JS from native code.
- Add relative path support to `include`.
- Add script caching to to `include` (measure if there is any performance improvement though).
- Add prototypes and `new` for JS objects where possible. Consider removing corresponding `Create*` interface methods. 
- Move setInterval and friends to global namespace.
- Port hacks from ui_hacks and wsh_mod. Also try to make minimize|maximize and etc utilize corresponding stock animations.
- Add ES6 modules support (`import`).
- Replace PropertyList with something more maintenable.
- Save Panel Properties dialog size between calls.
- See if it's possible to export some common Array checking/creating/parsing code.
- Add support for more UTF encodings when reading scripts and files (UTF16BE, UTF32LE, UTF32BE).
- Add a custom message support to profiler.
- Add W3C specified printf-lile behaviour to console.log.
- Add font caching (check if it's not handled by the system already though).
- Add a better drag-n-drop window (rip it from CUI?).
- Run a static analysis on the code (cppcheck? pvs? VS built-in?).
- Check remaining TODOs.

# Maybe
- Add CI (appveyor?).
- Follow the bug https://bugzilla.mozilla.org/show_bug.cgi?id=1474914, if fixed consider moving to newer non-ESR branches.
- Update scintilla JS keywords list.
- Raise warning level to W4 or WALL.
- Some sort of build system?
- Add help to properties and methods (JS_DefineFunctionsWithHelp)
- Add docs to ActiveXObject methods and properties.
- Update Brett's JS smooth playlist.
- Investigate: separate native JSON parser vs one from SpiderMonkey engine.
- Minimal .js tests for all the functionality: might be possible to cycle them in GUI via include().