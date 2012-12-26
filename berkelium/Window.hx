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
			trace("init");
			init = true;
			Lib.load("hxberkelium", "bk_init", 0)();
		}
		
		hnd = create_window(width, height, function(arr:Array<Dynamic>) this.onPaint(arr[0],arr[1],arr[2],arr[3],arr[4]));
	}
	
	public function navigate(to:String):Void
	{
		bk_navigate(hnd, to);
	}
	
	public static function update():Void
	{
		bk_update();
	}
	
	public dynamic function onPaint(buf:Dynamic, left:Int, top:Int, right:Int, bottom:Int):Void
	{
		trace("onPaint " + [left, top, right, bottom]);
	}
	
	static var bk_update = Lib.load("hxberkelium", "bk_update", 0);
	static var create_window = Lib.load("hxberkelium", "bk_create_window", 3);
	static var bk_navigate = Lib.load("hxberkelium", "bk_navigate", 2);
}
