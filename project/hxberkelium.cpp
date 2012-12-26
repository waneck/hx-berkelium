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
		
		virtual void onPaint(Berkelium::Window *wini,
			const unsigned char *bitmap_in, const Berkelium::Rect &bitmap_rect,
			size_t num_copy_rects, const Berkelium::Rect *copy_rects,
			int dx, int dy, const Berkelium::Rect &scroll_rect) {
				if (NULL != onPaintHandler && NULL != onPaintHandler->get())
				{
					value paintHandler = onPaintHandler->get();
					if (!val_is_null(paintHandler))
					{
						val_check_function(paintHandler, -1);
						int len = (bitmap_rect.right() - bitmap_rect.left()) * (bitmap_rect.bottom() - bitmap_rect.top());
						printf("len %i\n", len);
						
						buffer buf = alloc_buffer_len( len );
						buffer_append_sub(buf, (const char *) bitmap_in, len);
						value args[] = { buffer_val(buf), alloc_int(bitmap_rect.left()), alloc_int(bitmap_rect.top()), alloc_int(bitmap_rect.right()), alloc_int(bitmap_rect.bottom()) };
						val_callN(paintHandler, args, 5);
					}
				}
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
	w->getWindow()->navigateTo(val_string(to), val_strlen(to));
	
	return alloc_null();
}

DEFINE_PRIM(bk_navigate, 2);

// Reference this to bring in all the symbols for the static library
int hxberkelium_register_prims() { return 0; }

#ifdef __cplusplus
}
#endif