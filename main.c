#include <glib-2.0/glib.h>
#include <gio/gio.h>
#include <gtk/gtk.h>

// 7 15 22 29下午
static void async_ready_cb(GObject *object, GAsyncResult *res, GtkBuilder *builder) {
	GFile *file = G_FILE (object);
	GError *err = NULL;
	g_file_mount_enclosing_volume_finish (file, res, &err);

	if (err){
		g_message ("%s",err->message);
		
		GtkLabel *status_label = GTK_LABEL (gtk_builder_get_object(builder, "status_hint_label"));
		gtk_label_set_text (status_label, err->message);
		
		g_error_free (err);
	}

	GFileEnumerator *enumerator = g_file_enumerate_children(file,
			G_FILE_ATTRIBUTE_STANDARD_NAME,
			G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS,
			NULL,
			NULL);

	GtkWidget *list_box = gtk_list_box_new ();

	if (enumerator) {
		int count = 0;
		GFileInfo *info;
		while ((info = g_file_enumerator_next_file (enumerator, NULL, NULL)) != NULL) {
			g_message ("%s",g_file_info_get_name (info));
			count++;

			GtkWidget *name_label = gtk_label_new (g_file_info_get_name (info));
			gtk_list_box_prepend (GTK_LIST_BOX (list_box), name_label);
			g_object_unref(info);
		}

		if (count > 0) {
			GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
			gtk_container_add (GTK_CONTAINER (window), list_box);
			gtk_widget_show_all (window);
			g_message ("\n%d files", count);
		} else {
			g_object_unref (list_box);
		}
		g_object_unref(enumerator);
	}
}

static void login_bt_cb (GtkWidget *button, GtkBuilder *builder) {
	GtkEntry *host_entry = GTK_ENTRY (gtk_builder_get_object (builder, "host_entry"));
	GtkEntry *username_entry = GTK_ENTRY (gtk_builder_get_object (builder, "username_entry"));
	GtkEntry *pwd_entry = GTK_ENTRY (gtk_builder_get_object (builder, "pwd_entry"));
	const char* url = gtk_entry_get_text (host_entry);
	const char* user  = gtk_entry_get_text (username_entry);
	const char* pwd  = gtk_entry_get_text (pwd_entry);
	g_message ("login:%s, as user %s", url, user);
	
	GMountOperation *mount_op = g_mount_operation_new ();
	g_mount_operation_set_username (mount_op, user);
	g_mount_operation_set_password (mount_op, pwd);
	GFile *remote_file = g_file_new_for_uri (url);
	g_file_mount_enclosing_volume (remote_file,
			G_MOUNT_MOUNT_NONE,
			mount_op,
			NULL,
			(GAsyncReadyCallback) async_ready_cb,
			builder);
}

static void reset_bt_cb (GtkWidget *button, GtkBuilder *builder) {
	GtkEntry *host_entry = GTK_ENTRY (gtk_builder_get_object (builder, "host_entry"));
	GtkEntry *username_entry = GTK_ENTRY (gtk_builder_get_object (builder, "username_entry"));
	GtkEntry *pwd_entry = GTK_ENTRY (gtk_builder_get_object (builder, "pwd_entry"));
	GtkLabel *status_label = GTK_LABEL (gtk_builder_get_object(builder, "status_hint_label"));
	gtk_entry_set_text (host_entry, "");
	gtk_entry_set_text (username_entry, "");
	gtk_entry_set_text (pwd_entry, "");
	gtk_label_set_text (status_label, "dialog has been reseted");

}

int main (){
	gtk_init(NULL, NULL);

	GtkBuilder *builder = gtk_builder_new_from_file ("remote-login.ui");
	GtkWidget *top_window = GTK_WIDGET (gtk_builder_get_object (builder, "login_dialog"));

	GtkWidget *button_box = GTK_WIDGET (gtk_builder_get_object (builder, "button_box"));

	GtkWidget *login_button = gtk_button_new_with_label ("Login");
	gtk_container_add (GTK_CONTAINER (button_box), login_button);
	GtkWidget *reset_button = gtk_button_new_with_label ("Reset");
	gtk_container_add (GTK_CONTAINER (button_box), reset_button);

	g_signal_connect (GTK_BUTTON (login_button), "clicked", G_CALLBACK (login_bt_cb), builder);
	g_signal_connect (GTK_BUTTON (reset_button), "clicked", G_CALLBACK (reset_bt_cb), builder);

	g_signal_connect (top_window, "destroy", gtk_main_quit, NULL);

	gtk_widget_show_all (top_window);

	gtk_main ();
	return 0;
}

