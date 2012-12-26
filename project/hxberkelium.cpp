#ifndef STATIC_LINK
	#define IMPLEMENT_API
#endif

#if defined(HX_WINDOWS) || defined(HX_MACOS) || defined(HX_LINUX)
#define NEKO_COMPATIBLE
#endif
#include <hx/CFFI.h>
#include <berkelium/Berkelium.hpp>
#include <berkelium/Window.hpp>
#include <berkelium/WindowDelegate.hpp>
#include <berkelium/Context.hpp>

#include <cstring>
#include <string>
#include <iostream>

using namespace Berkelium;

class NMETextureWindow : public Berkelium::WindowDelegate {
public:
	NMETextureWindow(unsigned int _w, unsigned int _h, bool _usetrans, value paintHandler)
	     : width(_w),
	       height(_h),
	       needs_full_refresh(true)
		{
			onPaintHandler = new AutoGCRoot(paintHandler);
			Berkelium::Context *context = Berkelium::Context::create();
			bk_window = Berkelium::Window::create(context);
			delete context;
			bk_window->setDelegate(this);
			bk_window->resize(width, height);
			bk_window->setTransparent(_usetrans);
		}
	
		~NMETextureWindow() 
		{
			delete bk_window;
			delete onPaintHandler;
		}
	
		Berkelium::Window* getWindow() 
		{
			return bk_window;
		}
		
		virtual void onPaint(Window *win,
        const unsigned char *bitmap_in,
        const Rect &bitmap_rect,
        size_t numCopyRects,
        const Rect *copyRects,
        int dx, int dy,
        const Rect &scrollRect) {
				printf("on paint 1\n");
				if (NULL != onPaintHandler && NULL != onPaintHandler->get())
				{
					value paintHandler = onPaintHandler->get();
					printf("on paint 2\n");
					if (!val_is_null(paintHandler))
					{
						printf("on paint 3\n");
						val_check_function(paintHandler, -1);
						int len = (bitmap_rect.right() - bitmap_rect.left()) * (bitmap_rect.bottom() - bitmap_rect.top()) * 4;
						printf("len %i\n", len);
						
						buffer buf = alloc_buffer( 0 );
						printf("on paint 4\n");
						buffer_append_sub(buf, (const char *) bitmap_in, len);
						printf("on paint 5\n");
						
						value args = alloc_array(5);
						
						val_array_set_i(args, 0, buffer_val(buf));
						val_array_set_i(args, 1, alloc_int(bitmap_rect.left()));
						val_array_set_i(args, 2, alloc_int(bitmap_rect.top()));
						val_array_set_i(args, 3, alloc_int(bitmap_rect.right()));
						val_array_set_i(args, 4, alloc_int(bitmap_rect.bottom()));
						
						printf("on paint 6\n");
						val_call1(paintHandler, args);
						printf("on paint 7\n");
					}
				}
		}
	
	    virtual void onAddressBarChanged(Window *win, URLString newURL) {
	        std::cout << (void*)win << "*** onAddressBarChanged " << newURL << std::endl;
	    }
	    virtual void onStartLoading(Window *win, URLString newURL) {
	        std::cout << (void*)win << "*** onStartLoading " << newURL << std::endl;
	    }
	    virtual void onLoad(Window *win) {
	        std::wcout << (void*)win << L"*** onLoad " << std::endl;
	    }
	    virtual void onCrashedWorker(Window *win) {
	        std::wcout << (void*)win << L"*** onCrashedWorker " << std::endl;
	    }
	    virtual void onCrashedPlugin(Window *win, WideString pluginName) {
	        std::wcout << L"*** onCrashedPlugin " << pluginName << std::endl;
	    }
	    virtual void onProvisionalLoadError(Window *win, URLString url,
	                                        int errorCode, bool isMainFrame) {
	        std::cout << "*** onProvisionalLoadError " << url << ": " << errorCode;
	        if (isMainFrame) std::cout << " (main frame)";
	        std::cout << std::endl;
	    }
	    virtual void onConsoleMessage(Window *win, WideString message,
	                                  WideString sourceId, int line_no) {
	        std::wcout << L"*** onConsoleMessage " << message << L" from "
	                   << sourceId << L" line " << line_no << std::endl;
	    }
	    virtual void onScriptAlert(Window *win, WideString message,
	                              WideString defaultValue, URLString url,
	                              int flags, bool &success, WideString &value) {
	        std::wcout << L"*** onScriptAlert " << message << std::endl;
	    }
	    virtual void onNavigationRequested(Window *win, URLString newURL,
	                                       URLString referrer, bool isNewWindow,
	                                       bool &cancelDefaultAction) {
	        std::cout << (void*)win << "*** onNavigationRequested " << newURL << " by " << referrer
	                  << (isNewWindow?"  (new window)" : " (same window)") << std::endl;
	    }
	    virtual void onLoadingStateChanged(Window *win, bool isLoading) {
	        std::cout << (void*)win << "*** onLoadingStateChanged "
	                  << (isLoading?"started":"stopped") << std::endl;
	    }
	    virtual void onTitleChanged(Window *win, WideString title) {
	        std::wcout << L"*** onTitleChanged " << title << std::endl;
	    }
	    virtual void onTooltipChanged(Window *win, WideString text) {
	        std::wcout << L"*** onTooltipChanged " << text << std::endl;
	    }
	    virtual void onCrashed(Window *win) {
	        std::cout << "*** onCrashed " << std::endl;
	    }
	    virtual void onUnresponsive(Window *win) {
	        std::cout << "*** onUnresponsive " << std::endl;
	    }
	    virtual void onResponsive(Window *win) {
	        std::cout << "*** onResponsive " << std::endl;
	    }
	    virtual void onCreatedWindow(Window *win, Window *newWindow,
	                                 const Rect &initialRect) {
	        std::cout << (void*)win << "*** onCreatedWindow " << (void*)newWindow<<" "
	                  << initialRect.mLeft << "," << initialRect.mTop << ": "
	                  << initialRect.mWidth << "x" << initialRect.mHeight << std::endl;
	        if (initialRect.mWidth < 1 || initialRect.mHeight < 1) {
	            newWindow->resize(this->width, this->height);
	        }
	        newWindow->setDelegate(this);
	    }
	    virtual void onWidgetCreated(Window *win, Widget *newWidget, int zIndex) {
	        std::cout << "*** onWidgetCreated " << newWidget << " index " << zIndex << std::endl;
	    }
	    virtual void onWidgetResize(Window *win, Widget *wid, int newWidth, int newHeight) {
	        std::cout << "*** onWidgetResize " << wid << " "
	                  << newWidth << "x" << newHeight << std::endl;
	    }
	    virtual void onWidgetMove(Window *win, Widget *wid, int newX, int newY) {
	        std::cout << "*** onWidgetMove " << wid << " "
	                  << newX << "," << newY << std::endl;
	    }
	    virtual void onShowContextMenu(Window *win,
	                                   const ContextMenuEventArgs& args) {
	        std::cout << "*** onShowContextMenu at " << args.mouseX << "," << args.mouseY;
	        std::cout << "; type: ";
	        switch (args.mediaType) {
	          case ContextMenuEventArgs::MediaTypeImage:
	            std::cout << "image";
	            break;
	          case ContextMenuEventArgs::MediaTypeVideo:
	            std::cout << "video";
	            break;
	          case ContextMenuEventArgs::MediaTypeAudio:
	            std::cout << "audio";
	            break;
	          default:
	            std::cout << "other";
	        }
	        if (args.isEditable || args.editFlags) {
	            std::cout << " (";
	            if (args.isEditable)
	                std::cout << "editable; ";
	            if (args.editFlags & ContextMenuEventArgs::CanUndo)
	                std::cout << "Undo, ";
	            if (args.editFlags & ContextMenuEventArgs::CanRedo)
	                std::cout << "Redo, ";
	            if (args.editFlags & ContextMenuEventArgs::CanCut)
	                std::cout << "Cut, ";
	            if (args.editFlags & ContextMenuEventArgs::CanCopy)
	                std::cout << "Copy, ";
	            if (args.editFlags & ContextMenuEventArgs::CanPaste)
	                std::cout << "Paste, ";
	            if (args.editFlags & ContextMenuEventArgs::CanDelete)
	                std::cout << "Delete, ";
	            if (args.editFlags & ContextMenuEventArgs::CanSelectAll)
	                std::cout << "Select All";
	            std::cout << ")";
	        }
	        std::cout << std::endl;
	        if (args.linkUrl.length())
	            std::cout << "        Link URL: " << args.linkUrl << std::endl;
	        if (args.srcUrl.length())
	            std::cout << "        Source URL: " << args.srcUrl << std::endl;
	        if (args.pageUrl.length())
	            std::cout << "        Page URL: " << args.pageUrl << std::endl;
	        if (args.frameUrl.length())
	            std::cout << "        Frame URL: " << args.frameUrl << std::endl;
	        if (args.selectedText.length())
	            std::wcout << L"        Selected Text: " << args.selectedText << std::endl;
	    }
/*
	    virtual void onJavascriptCallback(Window *win, void* replyMsg, URLString url, WideString funcName, Script::Variant *args, size_t numArgs) {
	        std::cout << "*** onJavascriptCallback at URL " << url << ", "
	                  << (replyMsg?"synchronous":"async") << std::endl;
	        std::wcout << L"    Function name: " << funcName << std::endl;
	        for (size_t i = 0; i < numArgs; i++) {
	            WideString jsonStr = toJSON(args[i]);
	            std::wcout << L"    Argument " << i << ": ";
	            if (args[i].type() == Script::Variant::JSSTRING) {
	                std::wcout << L"(string) " << args[i].toString() << std::endl;
	            } else {
	                std::wcout << jsonStr << std::endl;
	            }
	            Script::toJSON_free(jsonStr);
	        }
	        if (replyMsg) {
	            win->synchronousScriptReturn(replyMsg, numArgs ? args[0] : Script::Variant());
	        }
	    }

	    /** Display a file chooser dialog, if necessary. The value to be returned should go ______.
	     * \param win  Window instance that fired this event.
	     * \param mode  Type of file chooser expected. See FileChooserType.
	     * \param title  Title for dialog. "Open" or "Save" should be used if empty.
	     * \param defaultFile  Default file to select in dialog.
	     *
	    virtual void onRunFileChooser(Window *win, int mode, WideString title, FileString defaultFile) {
	        std::wcout << L"*** onRunFileChooser type " << mode << L", title " << title << L":" << std::endl;
	#ifdef _WIN32
	        std::wcout <<
	#else
	        std::cout <<
	#endif
	            defaultFile << std::endl;

	        win->filesSelected(NULL);
	    }*/

	    virtual void onExternalHost(
	        Berkelium::Window *win,
	        Berkelium::WideString message,
	        Berkelium::URLString origin,
	        Berkelium::URLString target)
	    {
	        std::cout << "*** onExternalHost at URL from "<<origin<<" to "<<target<<":"<<std::endl;
	        std::wcout << message<<std::endl;
	    }

	    Berkelium::Window* window() const {
	        return bk_window;
	    }
		
private:
	AutoGCRoot *onPaintHandler;
	
	// The Berkelium window, i.e. our web page
	Berkelium::Window* bk_window;
	// Width and height of our window.
	unsigned int width, height;
	// Storage for a texture
	unsigned int web_texture;
	// Bool indicating when we need to refresh the entire image
	bool needs_full_refresh;
	
};

DEFINE_KIND(k_window);

#ifdef __cplusplus
extern "C" 
{
#endif

value bk_init()
{
	Berkelium::init(Berkelium::FileString::empty());
	return alloc_bool(true);
}

DEFINE_PRIM(bk_init, 0);

value bk_create_window(value _w, value _h, value paintHandler)
{
	val_check(_w, int);
	val_check(_h, int);
	
	return alloc_abstract(k_window, new NMETextureWindow(val_int(_w), val_int(_h), true, paintHandler));
}

DEFINE_PRIM(bk_create_window, 3);

value bk_navigate(value window, value to)
{
	val_check(to, string);
	val_check_kind(window, k_window);
	
	NMETextureWindow *w = (NMETextureWindow *) val_data(window);
	printf("nav 1\n");
	w->getWindow()->navigateTo(val_string(to), val_strlen(to));
	printf("nav 2\n");
	
	return alloc_null();
}

DEFINE_PRIM(bk_navigate, 2);

value bk_update()
{
	Berkelium::update();
}

DEFINE_PRIM(bk_update, 0);

value bk_destroy()
{
	Berkelium::destroy();
}

DEFINE_PRIM(bk_destroy, 0);

// Reference this to bring in all the symbols for the static library
int hxberkelium_register_prims() { return 0; }

#ifdef __cplusplus
}
#endif