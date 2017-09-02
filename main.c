#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <time.h>
#include <string.h>

#define MAX 100

#define dest_width 1000/2.5
#define dest_height 1100/2.5

struct Member{	//用户结构体
	int id;        //用户ID号
	int layer;     //堆木头的层数
	int length;    //总共堆木头的长度
	int minutes;  //分钟
	int seconds;   //秒
}member[MAX]={0};
struct Wood{	//木头结构体
	int x;   //木头左上端点的横坐标
	int y;   //木头左上端点的纵坐标
	int wid;    //长
	int len;    //宽
	GdkColor color; //木头的颜色
}wd[MAX];

struct User{
	char id[4];
	char name[20];
	char pass[7];
}User[MAX];

GtkWidget *button;
GtkWidget *table;

GtkWidget *label_id1;//玩家ID
GtkWidget *label_id2;

GtkWidget *label_layer1;//层数
GtkWidget *label_layer2;

GtkWidget *label_lenth1;//长度
GtkWidget *label_lenth2;

GtkWidget *label_time1;//分数
GtkWidget *label_time2;

GtkWidget *draw_erea;
int score;//总分
int layer;//层数
int lenth;//长度

int time_id;
GdkColor color;
GdkColor color_white;
GdkDrawable *draw_able;
GdkGC *gc;
GdkGC *gc_white;
GtkWidget *window;

GtkWidget *login_window;
GtkWidget *image;
const GdkPixbuf *src_pixbuf;
GdkPixbuf *dest_pixbuf;

PangoFontDescription *font;//字体

GtkWidget *name_entry;
GtkWidget *pass_entry;

GtkWidget *entry_id_r;
GtkWidget *entry_name_r;
GtkWidget *entry_pass1_r;
GtkWidget *entry_pass2_r;

FILE *f_userinfo;

int time_cou;
gint hour = 0;
gint min = 0;
gint sec = 0;
int test=0;
int down=0;
gboolean flag=TRUE;
int wnum=1;
int color_list[11]={65535,52685,21331,35723,57825,34695,44461,49344,24415,4626,32639};
char Idnumber[10] = "0123456789";
void InitWood(int i)//初始化木块
{
	if(i==0)//底部木头的初始化
	{
		wd[i].x=200;
		wd[i].y=620;
		wd[i].wid=20;
		wd[i].len=100;
	}
	else
	{
		wd[i].x=0;
		wd[i].y=20;
		wd[i].wid=20;
		wd[i].len=wd[i-1].len;
	}
	wd[i].color.blue=color_list[rand()%11];
	wd[i].color.red=color_list[rand()%11];
	wd[i].color.green=color_list[rand()%11];
}

void init()//游戏初始化
{
    int i,j,k;
    score=0;
    wnum=1;
    color_white.blue=65535;
    color_white.red=65535;
    color_white.green=65535;
    //color.blue=2345;
    //color.red=2523;
    //color.green=3453;
    gc=gdk_gc_new(draw_erea->window);
    gc_white=gdk_gc_new(draw_erea->window);
    gdk_gc_set_rgb_fg_color(gc,&color);
    gdk_gc_set_rgb_fg_color(gc_white,&color_white);
    draw_able=draw_erea->window;
    InitWood(0);
    InitWood(wnum);
}

void display_map()//显示map的block
{
    int i,j;
    gdk_draw_rectangle(draw_able,gc_white,TRUE,0,0,500,640);


    for(i=0;i<wnum;i++)
    {
        gdk_gc_set_rgb_fg_color(gc,&wd[i].color);
        gdk_draw_rectangle(draw_able,gc,TRUE,wd[i].x,wd[i].y,wd[i].len,wd[i].wid);
    }


    if(!down)
    {
        if(flag)
        {
            wd[wnum].x++;
            gdk_gc_set_rgb_fg_color(gc,&wd[wnum].color);
            gdk_draw_rectangle(draw_able,gc,TRUE,wd[wnum].x,wd[wnum].y,wd[wnum].len,wd[wnum].wid);
            if(wd[wnum].x==500-wd[wnum].len)flag=FALSE;
        }
        if(!flag)
        {
            wd[wnum].x--;
            gdk_gc_set_rgb_fg_color(gc,&wd[wnum].color);
            gdk_draw_rectangle(draw_able,gc,TRUE,wd[wnum].x,wd[wnum].y,wd[wnum].len,wd[wnum].wid);
            if(wd[wnum].x==0)flag=TRUE;
        }
    }
    else
    {
        wd[wnum].y=wd[wnum].y+10;
        gdk_gc_set_rgb_fg_color(gc,&wd[wnum].color);
        gdk_draw_rectangle(draw_able,gc,TRUE,wd[wnum].x,wd[wnum].y,wd[wnum].len,wd[wnum].wid);

        if(wd[wnum].y==620-wnum*20)
        {
            wnum++;
            InitWood(wnum);
            down=0;
        }
    }

}

void display_block()
{



}

void un_display_block()
{


}

void timeout()
{
    un_display_block();
    //down();

    display_map();
    display_block();

    g_source_remove(time_id);
    //sleep(1);
    time_id=g_timeout_add(100,(GtkFunction)timeout,NULL);
    return ;
}

gboolean expose_event_callback(GtkWidget *widget,GdkEventExpose *event,gpointer data)
{
    display_map();
    display_block();
}

gboolean key_value(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
    un_display_block();
    if(strcmp(gdk_keyval_name(event->keyval),"Up")==0)
    {
        printf("up\n");
    }
    else if(strcmp(gdk_keyval_name(event->keyval),"Down")==0)
    {
         printf("Down\n");
         down=1;
    }
    else if(strcmp(gdk_keyval_name(event->keyval),"Left")==0)
    {
         printf("Left\n");
    }
    else if(strcmp(gdk_keyval_name(event->keyval),"Right")==0)
    {
         printf("Right\n");
    }
    display_block();
    return FALSE;
}

void timer_add()
{
    gchar buf[100];
    gchar format[100];
    sec++;
    if (sec == 60) {
        min++;
        if (min == 60) {
            hour++;
            if (hour == 25) {
                hour = 0;
            }
            min = 0;
        }
        sec = 0;
    }
    sprintf(buf, "%02d:%02d:%02d", hour, min, sec);
    //gtk_label_set_text(GTK_LABEL(our_timer), buf);
    //printf( "%02d:%02d:%02d\n", hour, min, sec);
    gtk_label_set_text(GTK_LABEL(label_time2), buf);
    sprintf(format,"<span font_desc='15'>%02d:%02d:%02d</span>", hour, min, sec);
    gtk_label_set_markup(GTK_LABEL(label_time2),format);
    gtk_widget_modify_font(GTK_WIDGET(label_time2), font);
}

void timer_begin()
{

    gtk_label_set_markup(GTK_LABEL(label_time2),"<span font_desc='15'>00:00:01</span>");
    gtk_widget_modify_font(GTK_WIDGET(label_time2), font);
    sec = 1;
    min = 0;
    hour = 0;
    g_source_remove(time_cou);
    sleep(1);
    time_cou = gtk_timeout_add(1000, (GtkFunction)timer_add, NULL);
}

GdkPixbuf *create_pixbuf(const gchar * filename)
{
    GdkPixbuf *pixbuf;
    GError *error = NULL;
    /*
     * 函数gdk_pixbuf_new_from_file() 从一个图片文件中加载图象数据，从而生成一个新的 pixbuf，
     * 至于文件中包含图象的格式，是由系统自动检测的。如果该函数返回值是NULL 的话，程序就会出现错误。
    */
    pixbuf = gdk_pixbuf_new_from_file(filename, &error);
    if(!pixbuf) {
        fprintf(stderr, "%s\n", error->message);
        g_error_free(error);
    }
    return pixbuf;
}
void Game_show(gchar *uid)
{
    gchar format[100];
    window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GAME---WOOD");
    gtk_window_set_icon(GTK_WINDOW(window), create_pixbuf("./image/ico.jpg"));
    gtk_window_set_resizable (GTK_WINDOW (window), FALSE);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);


    table=gtk_table_new(8,2,FALSE);
    draw_erea=gtk_drawing_area_new();
    //玩家ID
    label_id1=gtk_label_new(g_locale_to_utf8("Your ID:",-1,NULL,NULL,NULL));
    label_id2=gtk_label_new(g_locale_to_utf8(uid,-1,NULL,NULL,NULL));
    //层数
    label_layer1=gtk_label_new(g_locale_to_utf8("Layer:",-1,NULL,NULL,NULL));
    label_layer2=gtk_label_new(g_locale_to_utf8("0",-1,NULL,NULL,NULL));
    //长度
    label_lenth1=gtk_label_new(g_locale_to_utf8("Lenth:",-1,NULL,NULL,NULL));
    label_lenth2=gtk_label_new(g_locale_to_utf8("0",-1,NULL,NULL,NULL));
    //时间
    label_time1=gtk_label_new(g_locale_to_utf8("Time:",-1,NULL,NULL,NULL));
    label_time2=gtk_label_new(g_locale_to_utf8("00:00:00",-1,NULL,NULL,NULL));

    gtk_widget_modify_font(GTK_WIDGET(label_id1), font);
    gtk_widget_modify_font(GTK_WIDGET(label_id2), font);
    gtk_widget_modify_font(GTK_WIDGET(label_layer1), font);
    gtk_widget_modify_font(GTK_WIDGET(label_layer2), font);
    gtk_widget_modify_font(GTK_WIDGET(label_lenth1), font);
    gtk_widget_modify_font(GTK_WIDGET(label_lenth2), font);
    gtk_widget_modify_font(GTK_WIDGET(label_time1), font);
    gtk_widget_modify_font(GTK_WIDGET(label_time2), font);

    sprintf(format,"<span font_desc='15'>%s</span>", uid);
    gtk_label_set_markup(GTK_LABEL(label_id2),format);


    gtk_label_set_markup(GTK_LABEL(label_id1),"<span font_desc='15'>Your ID:</span>");
    //gtk_label_set_markup(GTK_LABEL(label_id2),"<span font_desc='15'>%s</span>",uid);
    gtk_label_set_markup(GTK_LABEL(label_layer1),"<span font_desc='15'>Layer:</span>");
    gtk_label_set_markup(GTK_LABEL(label_layer2),"<span font_desc='15'>0</span>");
    gtk_label_set_markup(GTK_LABEL(label_lenth1),"<span font_desc='15'>Lenth:</span>");
    gtk_label_set_markup(GTK_LABEL(label_lenth2),"<span font_desc='15'>0</span>");
    gtk_label_set_markup(GTK_LABEL(label_time1),"<span font_desc='15'>Time:</span>");
    gtk_label_set_markup(GTK_LABEL(label_time2),"<span font_desc='15'>00:00:00</span>");

    gtk_widget_set_size_request(draw_erea,500,640);//设置大小
    gtk_container_add(GTK_CONTAINER(window), table);
    gtk_container_set_border_width(GTK_CONTAINER(window),20);
    gtk_container_add(GTK_CONTAINER(window),table);

    gtk_table_attach(GTK_TABLE(table),draw_erea,0,1,0,8,
        (GtkAttachOptions)(0),(GtkAttachOptions)(0),0,0);

    gtk_table_attach(GTK_TABLE(table),label_id1,1,2,0,1,
        (GtkAttachOptions)(0),(GtkAttachOptions)(0),20,20);
    gtk_table_attach(GTK_TABLE(table),label_id2,1,2,1,2,
        (GtkAttachOptions)(0),(GtkAttachOptions)(0),20,20);

    gtk_table_attach(GTK_TABLE(table),label_layer1,1,2,2,3,
        (GtkAttachOptions)(0),(GtkAttachOptions)(0),20,20);
    gtk_table_attach(GTK_TABLE(table),label_layer2,1,2,3,4,
        (GtkAttachOptions)(0),(GtkAttachOptions)(0),20,20);

    gtk_table_attach(GTK_TABLE(table),label_lenth1,1,2,4,5,
        (GtkAttachOptions)(0),(GtkAttachOptions)(0),20,20);
    gtk_table_attach(GTK_TABLE(table),label_lenth2,1,2,5,6,
        (GtkAttachOptions)(0),(GtkAttachOptions)(0),20,20);

    gtk_table_attach(GTK_TABLE(table),label_time1,1,2,6,7,
        (GtkAttachOptions)(0),(GtkAttachOptions)(0),20,20);
    gtk_table_attach(GTK_TABLE(table),label_time2,1,2,7,8,
        (GtkAttachOptions)(0),(GtkAttachOptions)(0),20,20);


    g_signal_connect(G_OBJECT(draw_erea),"expose_event",G_CALLBACK(expose_event_callback),NULL);//重绘事件
    g_signal_connect(G_OBJECT(window),"key-press-event",G_CALLBACK(key_value),NULL);
    g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK(gtk_main_quit),NULL);

   // time_cou=gtk_timeout_add(1000, (GtkFunction)timer_begin, NULL);
    //time_id=g_timeout_add(1000,(GtkFunction)timeout,NULL);

    gtk_widget_show_all(window);

    init();

    /*线程的初始化*/
    if(!g_thread_supported()) g_thread_init(NULL);
    gdk_threads_init();
    /*创建线程*/
    g_thread_create((GThreadFunc)timer_begin, NULL, FALSE, NULL);
    g_thread_create((GThreadFunc)timeout, NULL, FALSE, NULL);

    gdk_threads_enter();
    gtk_main ();
    gdk_threads_leave();
}
void loginfunc(GtkWidget *widget, gpointer *window)
{
    const gchar *uid;
    const gchar *upasswd;
    int i=0;
    int id_temp;
    GtkWidget *dialog;

    uid=gtk_entry_get_text(GTK_ENTRY(name_entry));/*获得用户名*/
    upasswd=gtk_entry_get_text(GTK_ENTRY(pass_entry));/*获得密码*/

    if((f_userinfo=fopen("user_info.txt","r"))==NULL)
    {
        dialog = gtk_message_dialog_new(window,
                GTK_DIALOG_DESTROY_WITH_PARENT,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_OK,
                "Please check user name or password!");
                gtk_window_set_title(GTK_WINDOW(dialog), "Error");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
    else
    {
        while(!feof(f_userinfo))
        {
            i++;
            if(fscanf(f_userinfo,"%s%s%s",User[i].id,User[i].name,User[i].pass) <= 0)
            {
                break;
                i--;
            }
        }
        while(i)
        {
            if(i==0) break;

            if(!strcmp(User[i].id,uid))//一样的id
            {
               id_temp=i;
               break;
            }
            i--;
        }
        if(!strcmp(User[id_temp].pass,upasswd))
        {
            gtk_widget_hide(window);
            Game_show(uid);
        }
        else
        {
            dialog = gtk_message_dialog_new(window,
                    GTK_DIALOG_DESTROY_WITH_PARENT,
                    GTK_MESSAGE_ERROR,
                    GTK_BUTTONS_OK,
                    "Please check user name or password!");
                    gtk_window_set_title(GTK_WINDOW(dialog), "Error");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
        }
    }
}

void checkedfunc(GtkWidget *widget, gpointer *window)
{
    int i=0;
    int j;
    char uid[4];
    const gchar *uname;
    const gchar *upasswd1;
    const gchar *upasswd2;
    GtkWidget *dialog;
    int flag=1;
    int flag1=1;
    int Unumber=0;
    char printinfo[50];

    uname=gtk_entry_get_text(GTK_ENTRY(entry_name_r));/*获得用户名*/
    upasswd1=gtk_entry_get_text(GTK_ENTRY(entry_pass1_r));/*获得密码1*/
    upasswd2=gtk_entry_get_text(GTK_ENTRY(entry_pass2_r));/*获得密码2*/
    if(strlen(upasswd1)!=6||strcmp(upasswd1,upasswd2))
    {
         dialog = gtk_message_dialog_new(window,
                    GTK_DIALOG_DESTROY_WITH_PARENT,
                    GTK_MESSAGE_ERROR,
                    GTK_BUTTONS_OK,
                    "Error Password!");
                    gtk_window_set_title(GTK_WINDOW(dialog), "Error");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
    }
    else
    {
         if((f_userinfo=fopen("user_info.txt","a"))==NULL)
         {
            dialog = gtk_message_dialog_new(window,
                    GTK_DIALOG_DESTROY_WITH_PARENT,
                    GTK_MESSAGE_ERROR,
                    GTK_BUTTONS_OK,
                    "Error loading file!");
                    gtk_window_set_title(GTK_WINDOW(dialog), "Error");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
         }

        while(!feof(f_userinfo))
        {
            if(fscanf(f_userinfo,"%s%s%s",User[i+1].id,User[i+1].name,User[i+1].pass) <= 0)
                break;
            Unumber++;
            i++;
        }
        while(flag1)
        {
            if(flag)
            {
                for(j=0;j<3;j++)
                    uid[j]=rand()%10+'0';
                uid[3]='\0';
            }

            if(i==0) break;

            if(!strcmp(User[i].id,uid))//一样的id
            {
                flag=1;
                i=Unumber+1;
            }
            else{
                flag=0;
            }

            i--;
        }
        if(i==0)
        {
            fprintf(f_userinfo,"%s %s %s\r\n",uid,uname,upasswd1);
            sprintf(printinfo,"Registration Successful\nYour ID:%s\nYour Name:%s\nYour Password:%s\n",uid,uname,upasswd1);
            dialog = gtk_message_dialog_new(window,
                                    GTK_DIALOG_DESTROY_WITH_PARENT,
                                    GTK_MESSAGE_QUESTION,
                                    GTK_BUTTONS_OK,
                                    printinfo);

            gtk_window_set_title(GTK_WINDOW(dialog), "Success");
            gint result = gtk_dialog_run (GTK_DIALOG (dialog));
            switch (result)
            {
                case GTK_RESPONSE_OK:
                    fclose(f_userinfo);
                    gtk_widget_hide(window);
                break;
                default:

                break;
            }
            gtk_widget_destroy(dialog);
        }
    }

}

void canclefunc(GtkWidget *widget, gpointer *window)
{
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new(window,
    GTK_DIALOG_DESTROY_WITH_PARENT,
    GTK_MESSAGE_QUESTION,
    GTK_BUTTONS_YES_NO,
    "Are you sure to quit?");

    gtk_window_set_title(GTK_WINDOW(dialog), "Warning");

    gint result = gtk_dialog_run (GTK_DIALOG (dialog));
    switch (result)
    {
        case GTK_RESPONSE_YES:
            gtk_widget_hide(window);
        break;
        default:

        break;
    }
    gtk_widget_destroy(dialog);
}

void regfunc(GtkWidget *widget, gpointer *data)
{
    GtkWidget *reg_window;

    //GtkWidget *label_id_r;//玩家ID
    GtkWidget *label_name_r;//玩家
    GtkWidget *label_pass1_r;//玩家
    GtkWidget *label_pass2_r;//玩家

    GtkWidget *button_enter=gtk_button_new_with_label("Ensure");
    GtkWidget *button_cancle=gtk_button_new_with_label("Cancel");

    GtkWidget *table=gtk_table_new(5,2,FALSE);

    reg_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    //gtk_window_set_decorated(GTK_WINDOW(window), FALSE);       // 设置无边框
    gtk_window_set_default_size(GTK_WINDOW(reg_window),600,480);
    gtk_window_set_title(GTK_WINDOW(reg_window), "GAME---WOOD");
    gtk_window_set_resizable (GTK_WINDOW (reg_window), FALSE);
    gtk_window_set_icon(GTK_WINDOW(reg_window), create_pixbuf("./image/ico.jpg"));
    gtk_window_set_position(GTK_WINDOW(reg_window), GTK_WIN_POS_CENTER);


    //label_id_r=gtk_label_new(g_locale_to_utf8("Input ID:\n(Your ID with 3 bits!)",-1,NULL,NULL,NULL));
    label_name_r=gtk_label_new(g_locale_to_utf8("Input Name:",-1,NULL,NULL,NULL));
    label_pass1_r=gtk_label_new(g_locale_to_utf8("Input Password:\n(Your Password with 6 bits!)",-1,NULL,NULL,NULL));
    label_pass2_r=gtk_label_new(g_locale_to_utf8("Input Password Again:",-1,NULL,NULL,NULL));

    //entry_id_r=gtk_entry_new_with_max_length(3);
    entry_name_r=gtk_entry_new();
    entry_pass1_r=gtk_entry_new_with_max_length(6);
    entry_pass2_r=gtk_entry_new_with_max_length(6);
     gtk_entry_set_visibility(GTK_WIDGET(entry_pass1_r),FALSE);
      gtk_entry_set_visibility(GTK_WIDGET(entry_pass2_r),FALSE);

    //gtk_editable_set_editable(GTK_WIDGET(entry_id_r),TRUE);
    gtk_editable_set_editable(GTK_WIDGET(entry_name_r),TRUE);
    gtk_editable_set_editable(GTK_WIDGET(entry_pass1_r),TRUE);
    gtk_editable_set_editable(GTK_WIDGET(entry_pass2_r),TRUE);

    //gtk_widget_modify_font(GTK_WIDGET(label_id_r), font);
    gtk_widget_modify_font(GTK_WIDGET(label_name_r), font);
    gtk_widget_modify_font(GTK_WIDGET(label_pass1_r), font);
    gtk_widget_modify_font(GTK_WIDGET(label_pass2_r), font);
    //gtk_widget_modify_font(GTK_WIDGET(entry_id_r), font);
    gtk_widget_modify_font(GTK_WIDGET(entry_name_r), font);
    gtk_widget_modify_font(GTK_WIDGET(entry_pass1_r), font);
    gtk_widget_modify_font(GTK_WIDGET(entry_pass2_r), font);

    gtk_container_add(GTK_CONTAINER(reg_window), table);
    gtk_container_set_border_width(GTK_CONTAINER(window),20);
    gtk_container_add(GTK_CONTAINER(window),table);

   // gtk_table_attach(GTK_TABLE(table),label_id_r,0,1,0,1,(GtkAttachOptions)(0),(GtkAttachOptions)(0),0,0);

    gtk_table_attach(GTK_TABLE(table),label_name_r,0,1,1,2,
        (GtkAttachOptions)(0),(GtkAttachOptions)(0),20,20);

    gtk_table_attach(GTK_TABLE(table),label_pass1_r,0,1,2,3,
        (GtkAttachOptions)(0),(GtkAttachOptions)(0),20,20);

    gtk_table_attach(GTK_TABLE(table),label_pass2_r,0,1,3,4,
        (GtkAttachOptions)(0),(GtkAttachOptions)(0),20,20);

    gtk_table_attach(GTK_TABLE(table),button_enter,0,1,4,5,
        (GtkAttachOptions)(0),(GtkAttachOptions)(0),20,20);

    //gtk_table_attach(GTK_TABLE(table),entry_id_r,1,2,0,1,(GtkAttachOptions)(0),(GtkAttachOptions)(0),20,20);

    gtk_table_attach(GTK_TABLE(table),entry_name_r,1,2,1,2,
        (GtkAttachOptions)(0),(GtkAttachOptions)(0),20,20);

    gtk_table_attach(GTK_TABLE(table),entry_pass1_r,1,2,2,3,
        (GtkAttachOptions)(0),(GtkAttachOptions)(0),20,20);

    gtk_table_attach(GTK_TABLE(table),entry_pass2_r,1,2,3,4,
        (GtkAttachOptions)(0),(GtkAttachOptions)(0),20,20);

    gtk_table_attach(GTK_TABLE(table),button_cancle,1,2,4,5,
        (GtkAttachOptions)(0),(GtkAttachOptions)(0),20,20);

    g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK(gtk_main_quit),NULL);
    g_signal_connect(GTK_OBJECT(button_enter), "clicked",GTK_SIGNAL_FUNC(checkedfunc), G_OBJECT(reg_window));
    g_signal_connect(GTK_OBJECT(button_cancle), "clicked",GTK_SIGNAL_FUNC(canclefunc), G_OBJECT(reg_window));

    gtk_widget_show_all(reg_window);
}

int main(int argc, char** argv)
{

    gtk_init(&argc,&argv);
    srand((unsigned)time(NULL));/*播种子*/
    font = pango_font_description_from_string("Consolas");          //"Sans"字体名
    char *image_path="./image/first_back.png";

     GtkWidget *log_button= gtk_button_new();
    GtkWidget *reg_button= gtk_button_new();

    GtkWidget *image3 = gtk_image_new_from_file(image_path);
    GtkWidget *log_image = gtk_image_new_from_file("./image/log.png");
    GtkWidget *reg_image = gtk_image_new_from_file("./image/reg.png");

    GtkWidget *name_lable=gtk_label_new(g_locale_to_utf8("UserID:",-1,NULL,NULL,NULL));
    GtkWidget *pass_lable=gtk_label_new(g_locale_to_utf8("Password:",-1,NULL,NULL,NULL));

    name_entry = gtk_entry_new_with_max_length(3);
    pass_entry = gtk_entry_new_with_max_length(6);
    gtk_entry_set_visibility(GTK_WIDGET(pass_entry),FALSE);
    gtk_editable_set_editable(GTK_WIDGET(pass_entry),TRUE);
    gtk_editable_set_editable(GTK_WIDGET(name_entry),TRUE);

    login_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(login_window),600,480);
    gtk_window_set_title(GTK_WINDOW(login_window), "GAME---WOOD");
    gtk_window_set_resizable (GTK_WINDOW (login_window), FALSE);
    gtk_window_set_icon(GTK_WINDOW(login_window), create_pixbuf("./image/ico.jpg"));
    gtk_window_set_position(GTK_WINDOW(login_window), GTK_WIN_POS_CENTER);

    //gtk_window_set_opacity(GTK_WINDOW(login_window), 0.7); // 设置透明度函数
    //gtk_window_set_decorated(GTK_WINDOW(login_window), FALSE);//无边框

    GtkWidget * fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(login_window), fixed);

    gtk_fixed_put(GTK_FIXED(fixed), image3,0,0);

    gtk_widget_modify_font(GTK_WIDGET(name_lable), font);
    gtk_widget_modify_font(GTK_WIDGET(pass_lable), font);
    gtk_fixed_put(GTK_WIDGET(fixed), name_lable,140,180);
    gtk_fixed_put(GTK_WIDGET(fixed), pass_lable,140,230);

    gtk_widget_modify_font(GTK_WIDGET(name_entry), font);
    gtk_widget_modify_font(GTK_WIDGET(pass_entry), font);
    gtk_fixed_put(GTK_WIDGET(fixed), name_entry,140,200);
    gtk_fixed_put(GTK_WIDGET(fixed), pass_entry,140,250);

    gtk_fixed_put(GTK_WIDGET(fixed), log_button,140,290);
    gtk_container_add(GTK_CONTAINER(login_window), log_button);
    gtk_button_set_image(GTK_BUTTON(log_button), log_image);
    gtk_button_set_relief(GTK_BUTTON(log_button), GTK_RELIEF_NONE);

    gtk_fixed_put(GTK_WIDGET(fixed), reg_button,250,290);
    gtk_container_add(GTK_CONTAINER(login_window), reg_button);
    gtk_button_set_image(GTK_BUTTON(reg_button), reg_image);
    gtk_button_set_relief(GTK_BUTTON(reg_button), GTK_RELIEF_NONE);

    g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK(gtk_main_quit),NULL);
    g_signal_connect(GTK_OBJECT(log_button), "clicked",GTK_SIGNAL_FUNC(loginfunc), G_OBJECT(login_window));
    g_signal_connect(GTK_OBJECT(reg_button), "clicked",GTK_SIGNAL_FUNC(regfunc), NULL);

    gtk_widget_show(login_window);
    gtk_widget_show_all(login_window);

    gtk_main ();

    return (EXIT_SUCCESS);
}
