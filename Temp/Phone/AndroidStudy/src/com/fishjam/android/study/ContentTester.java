package com.fishjam.android.study;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.test.ActivityTestCase;
import android.test.AndroidTestCase;
import android.widget.ArrayAdapter;
import android.widget.AutoCompleteTextView;
import android.widget.ListView;
import android.widget.SimpleAdapter;


/***************************************************************************************************************************************
* Uri
*      Uri uri = Uri.parse("content://contacts/people/1");  
* UriMatcher 
*     UriMatcher sUriMatcher = new UriMatcher(UriMatcher.NO_MATCH);
*     sUriMatcher.addURI(Employees.AUTHORITY, "employee", EMPLOYEE);
*     sUriMatcher.addURI(Employees.AUTHORITY, "employee/#", EMPLOYEE_ID);
*     switch(sUriMatcher.match(uri)){ case EMPLOYEE_ID:  String noteId= uri.getPathSegments().get(1);  };
*     
* 系统内置了大量的URI
*   Contacts.People.CONTENT_URI --
***************************************************************************************************************************************/

/***************************************************************************************************************************************
 * Android提供了多种数据存取方式：
 *   Preference -- 数据较少的配置信息等， 使用键值对的方式保存在 XML 文件中 ( /Android/data/<package>/shared_prefs/ 目录下 )
 *      SharedPreferences --  Context.getSharedPreferences(文件名，模式).edit().putString("key","Value");  修改后需要 commit();
 *   File (默认保存在 /data/<package>/files 下面)
 *     Context.openFileInput   -- 获得标准的Java文件输入流(FileInputStream)，然后循环读取 
 *     Context.openFileOutput -- 获得标准的Java文件输出流(FileOutputStream)。fos.write(content.getBytes()); fos.close();
 *     Resources.openRawResource -- 返回 InputStream
 *   SQLite (数据文件一般保存在 /data/<package>/databases/ 下，创建或打开时需要使用全路径 )
 *     SQLiteDatabase -- 数据库管理类，控制 创建、增删改 等
 *     SQLiteOpenHelper -- 辅助类，管理数据库创建和版本更新。继承后重载 onCreate/onUpgrade 等方法，可通过 getWritableDatabase/getReadableDatabase 获取对应的数据库
 *     Cursor(游标接口，提供便利查询结果的方法)
 *   网络
***************************************************************************************************************************************/

/**************************************************************************************************************************************
 * 数据提供者(ContentProvider) -- 跨应用程序共享数据的唯一方法？ 对应的URI必须是 "content://" 开头的。
 *   Android中各个应用程序组件之间是相互独立的，彼此的数据不能共享(能访问其他程序的 DB/File 吗？)。 
 *     ContentProvider -- 检索和保存数据，允许一个应用程序访问其他应用程序（比如通信录）的数据，或者共享它自己的数据，其中包含了增删改、查询等操作
 *       如自定义时，需要重载的抽象方法: query/insert/update/delete/getType 等，然后在配置文件中声明。
 *       系统已经为常用的数据类型(音频、视频、图片、联系方式等)提供了大量的ContentProvider
 *       -- MediaStoreContentProvider -- 增加多媒体文件的操作的 ContentProvider
 *     ContentResolver -- 客户端访问时使用的类(Context.getContentResolver() )，其内部操作对应的 ContentProvider 来对数据进行操作。 
 *     
 * Adapter -- 适配器接口
 *   BaseAdapter -- 自定义适配器的基类，一般从该类继承，作为Module和View之间的桥梁。从该类继承可以取得对Adapter最大的控制权，
 *     实现 getCount, getItem{return null}, getView{ return myCustomView; } 等方法。可用于类似 Windows Virtual ListView 的高性能显示
 *   ArrayAdapter -- 常用于将数组或List集合的多个值包装成多个列表项，功能有限，其列表项只能是TextView，一般只用于 AutoCompleteTextView 等只显示文本的地方
 *   SimpleAdapter -- 可用于将List集合的多个对象包装成多个列表项，功能很强大

 *   SimpleCursorAdapter -- 包装Cursor提供的数据
 *     
 * Intent/IntentFilter -- 不同组件之间通信的载体
 *   显式Intent -- 明确指定需要启动或触发的组件的类名
 *   隐式Intent -- 只是指定需要启动或触发的组件应满足怎样的条件，Android系统解析出条件，并在系统中查找与之匹配的目标组件，如找到则启动或触发。
 *   
 *   启动类型和方式
 *     启动Activity -- Context.startActivity/startActivityForResult
 *     启动Service -- Context.startService/bindService
 *     发送广播 -- Context.sendBroadcast, sendStickyBroadcast, sendOrderedBroadcast

 **************************************************************************************************************************************/

public class ContentTester  extends ActivityTestCase{
	public void testArrayAdapter(){
		String [] booksStrings = {"book1" , "book2", "book3"};
		ArrayAdapter<String> adapter1 = new ArrayAdapter<String>(getActivity() , android.R.layout.simple_list_item_1, booksStrings);
		AutoCompleteTextView autoCompleteTextView = (AutoCompleteTextView)getActivity().findViewById(R.id.textView1);
		if (autoCompleteTextView != null) {
			autoCompleteTextView.setAdapter(adapter1);
		}
	}
	public void testSimpleAdapter(){
		List<Map<String, Object>> listItems = new ArrayList<Map<String, Object>>();
		for (int i = 0; i < 10; i++) {
			Map<String, Object> listItem = new HashMap<String, Object>();
			listItem.put("keyName", "myValue");
			listItems.add(listItem);
		}
		SimpleAdapter simpleAdapter = new SimpleAdapter(
				this.getActivity(), 
				listItems,	//List<? extends Map<String, ?>> 类型的集合对象，其中的每个 Map<String, ?> 对象生成一个列表项 
				android.R.layout.simple_list_item_checked, 	//一个界面布局ID，每个列表项都按照这个进行布局
				new String[]{"keyName"}, 	//String[] 的数组，决定提取Map<String,?>对象中哪些key对应的value来生成列表项
				new int[]{android.R.id.text1}	//int[] 类型的数组，决定对应的 Key 显示在界面布局中的哪个组件上
		);
		//ListView listView = (ListView)getActivity().findViewById(R.id.mylist);
		//listView.setAdapter(simpleAdapter);
	}
	public void ListActivityCursorTester(){
		
		/*********************************************************************************************************
		//class MyActivity extends ListActivity
		
		Cursor cursor = getContentResolver().query(People.CONTENT_URI, null, null, null, null);
		startManagingCursor(c);
		ListAdapter adapter = new SimpleCursorAdapter(this,  android.R.layout.simple_list_item_1,  c,  
				new String[] { People.NAME },  new int[] {android.R.id.textName } );
		setListAdapter(adapter);
		*********************************************************************************************************/
		
	}
	
	//根据用户名从联系人管理程序中查询
	public void QueryUserInfoByName(){
	/********************************************************************************************************
	//TODO: 1.使用的方式已经被淘汰，应该有更合适的方式; 2.需要增加 READ_CONTACTS 的权限
	 
		String name = "userName";
		//创建查询数组
		String [] projectionStrings = new String[] { People._ID, People.NAME, People.NUMBER };
		Uri contactsUri = People.CONTENT_URI;  //查询Uri对象
		String[] args = { name };	//查询参数
		//根据姓名查询联系人
		Cursor managedCursor = managedQuery(contactsUri, projectionStrings, "name=?", args, People.NAME, " ASC" );
		if(managedCursor.moveToFirst()){
			String name1 = managedCursor.getString(1); 	//获得姓名
			String number = managedCursor.getString(2); //获得电话号码
		}
	*******************************************************************************************************/
	}
	
	public void DialTelTester(){
	/*******************************************************************************************************
	    //显示拨打电话，如使用 Intent.ACTION_CALL 则直接拨打电话
		String data = "tel:15184464231";
		Uri uri = Uri.parse(data);
		Intent intent = new Intent();
		intent.setAction(Intent.ACTION_DIAL);
		intent.setData(uri);
		startActivity(intent);
	*******************************************************************************************************/
	}
}
