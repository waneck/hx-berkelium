package berkelium;
import cpp.Lib;

class Window
{
	static var init = false;
	
	private var hnd:Dynamic;
	
	public function new(width, height):Void
	{
		if (!init)
		{
			init = true;
			Lib.load("hxberkelium", "bk_init", 0)();
		}
		
		hnd = create_window(width, height, function(buf,left,top,right,bottom) this.onPaint(buf,left,top,right,bottom));
	}
	
	public function navigate(to:String):Void
	{
		bk_navigate(hnd, to);
	}
	
	public dynamic function onPaint(buf:haxe.io.BytesData, left:Int, top:Int, right:Int, bottom:Int):Void
	{
		trace("onPaint " + [left, top, right, bottom]);
	}
	
	
	static var create_window = Lib.load("hxberkelium", "bk_create_window", 3);
	static var bk_navigate = Lib.load("hxberkelium", "bk_navigate", 2);
}