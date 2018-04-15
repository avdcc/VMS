#include <gtk/gtk.h> 
#include <string.h>
#include <stdio.h>
#include "structs/types.h"


// gcc -o interface interface.c $(pkg-config --cflags --libs gtk+-3.0)

//-Globais-//

GtkWidget *window;
GtkWidget *grid;
GtkEntryBuffer *bufferS, *bufferConsole, *bufferInput;
GtkWidget *viewC;

typedef enum{PC, FP, SP, GP} Pointer;

GtkListStore *storeCode, *storeHeap, *storeOP, *storeCall;

void insCode(char*);
void insCall(char*);
void insOP(char*);
void insHeap(char*);

int ii = 0;  // mais tarde substituido pelo indicador do num de linhas em cada stack
int n = 0;

//-----------------------------------------------------------------------------//

void freeLine(char** line, int t){
    for(int i=0; i<t; i++) free(line[i]); 
}

void initLine(char** line, int t){
    for(int i=0; i<t; i++) line[i] = (char*)malloc(10 * sizeof(char)); 
}

void parseLine(char* line){
    if(!strncmp(line, "CO", 2)) insCode(line);
    else if(!strncmp(line, "CA", 2)) insCall(line);
    else if(!strncmp(line, "OP", 2)) insOP(line);
    else if(!strncmp(line, "HE", 2)) insHeap(line);
    //else if(!strncmp(line, "\e[", 2)) {;}
    //else if(strncmp(line, "OU", 2)){ }
}

//-----------------------------------------------------------------------------//

static char* GtkFileOpen () {
  GtkWidget *dialog;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
  gint res;

  dialog = gtk_file_chooser_dialog_new ("Open File", GTK_WINDOW (window), action, ("Cancel"), GTK_RESPONSE_CANCEL, ("Open"), GTK_RESPONSE_ACCEPT, NULL);

  res = gtk_dialog_run (GTK_DIALOG (dialog));
  if (res == GTK_RESPONSE_ACCEPT) {
      char *filename;
      GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
      filename = gtk_file_chooser_get_filename (chooser);
      gtk_widget_destroy (dialog);
      return(filename);
    }
  gtk_widget_destroy (dialog);
  return NULL;
}

//-----------------------------------------------------------------------------//

static void selecionar (char *i) {

  GtkTreeIter iter;
  GtkTreePath *path;
  path = gtk_tree_path_new_from_string (i);
  gtk_tree_model_get_iter(GTK_TREE_MODEL(storeCode), &iter, path);

  gtk_tree_view_set_cursor (GTK_TREE_VIEW (viewC), path, NULL, FALSE);
}

static void bExe (GtkWidget *widget, gpointer data) {
   fprintf(stdout,"1\n"); fflush(stdout);
}

static void bExeT (GtkWidget *widget, gpointer data) {
    int len;
    const char* nvezes = gtk_entry_buffer_get_text(bufferS);
    char line[MAX_LINE];
    // escrever n de instrucoes a executar
    fprintf(stdout, "%s\n", nvezes); fflush(stdout);
    // receber alteraçoes a fazer nas stacks
    do{
        fgets(line, MAX_LINE, stdin);
        parseLine(line);
    }
    while(line[0] != 'i' && line[0] != '\e');
}

static void bLoadPFile (GtkWidget *widget, gpointer data) {
    int len;
    char* filename = GtkFileOpen();
    char line[MAX_LINE];
    len = strnlen(filename ,MAX_LINE-1);
    filename[len++] = '\n';

    write(1, filename, len);
    do{
       fgets(line, MAX_LINE, stdin);
       parseLine(line);
    } while(line[0] == 'C');
    free(filename);
}

static void bLoadIFile (GtkWidget *widget, gpointer data) {
    char* filename = GtkFileOpen ();
    g_free (filename);
    g_print ("Click Load Input\n");
}

//-----------------------------------------------------------------------------//

void actLabel (Pointer lab, int value) {

  GtkWidget *label;
  char l[10], b[5];
  sprintf(b, "%d", value);
  switch (lab) {
    case PC:
      strncpy(l, "PC:", 10); strcat(l,b);
      label = gtk_label_new (l);
      gtk_grid_attach (GTK_GRID (grid), label, 12, 0, 1, 1); break;
    case FP:
      strncpy(l, "FP:", 10); strcat(l,b);
      label = gtk_label_new (l);
      gtk_grid_attach (GTK_GRID (grid), label, 13, 0, 1, 1); break;
    case SP:
      strncpy(l, "SP:", 10); strcat(l,b);
      label = gtk_label_new (l);
      gtk_grid_attach (GTK_GRID (grid), label, 14, 0, 1, 1); break;
    case GP:
      strncpy(l, "GP:", 10); strcat(l,b);
      label = gtk_label_new (l);
      gtk_grid_attach (GTK_GRID (grid), label, 15, 0, 1, 1); break;
    default :
    g_print("Erro label invalido\n" );
  }
}

    //-----------------------------------------//

static void activateLables () {

  GtkWidget *label;

  label = gtk_label_new ("Code");
  gtk_grid_attach (GTK_GRID (grid), label, 0, 0, 3, 1);

  label = gtk_label_new ("Heap");
  gtk_grid_attach (GTK_GRID (grid), label, 3, 0, 3, 1);

  label = gtk_label_new ("OPStack");
  gtk_grid_attach (GTK_GRID (grid), label, 6, 0, 3, 1);

  label = gtk_label_new ("Call Stack");
  gtk_grid_attach (GTK_GRID (grid), label, 9, 0, 3, 1);

  //-------------------------------------------//

  actLabel (PC, 0);
  actLabel (FP, 0);
  actLabel (SP, 0);
  actLabel (GP, 0);
}

//-----------------------------------------------------------------------------//

static void activateInputs () {

  GtkWidget *entry;

  //gtk_widget_set_hexpand (entry, FALSE);
  //gtk_widget_set_vexpand (entry, FALSE);

  bufferS = gtk_entry_buffer_new ("1", 3);
  entry = gtk_entry_new_with_buffer (bufferS);
  gtk_entry_set_max_length (GTK_ENTRY (entry), 5);
  gtk_grid_attach (GTK_GRID (grid), entry, 14, 2, 2, 1);

    //-----------------------------------------//

  GtkWidget *button;

  button = gtk_button_new_with_label ("Execute 1");
  g_signal_connect (button, "clicked", G_CALLBACK (bExe), NULL);
  gtk_grid_attach (GTK_GRID (grid), button, 12, 1, 4, 1);

  button = gtk_button_new_with_label ("Execute N:");
  g_signal_connect (button, "clicked", G_CALLBACK (bExeT), NULL);
  gtk_grid_attach (GTK_GRID (grid), button, 12, 2, 2, 1);

  button = gtk_button_new_with_label ("Load Program File");
  g_signal_connect (button, "clicked", G_CALLBACK (bLoadPFile), NULL);
  gtk_grid_attach (GTK_GRID (grid), button, 12, 3, 4, 1);

  button = gtk_button_new_with_label ("Load Input File");
  g_signal_connect (button, "clicked", G_CALLBACK (bLoadIFile), NULL);
  gtk_grid_attach (GTK_GRID (grid), button, 12, 4, 4, 1);

    //-----------------------------------------//

  GtkWidget *scrolled_window;
  GtkWidget *view;

  scrolled_window = gtk_scrolled_window_new (NULL, NULL);

  gtk_widget_set_hexpand (scrolled_window, FALSE);
  gtk_widget_set_vexpand (scrolled_window, FALSE);

  bufferInput = gtk_entry_buffer_new ("Input", 25);

  view = gtk_text_view_new ();
  //view = gtk_text_view_new_with_buffer (GTK_TEXT_BUFFER(bufferInput)); // <- CORRIGIR
  gtk_text_view_set_editable (GTK_TEXT_VIEW (view), TRUE);
  gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (view), TRUE);
  gtk_container_add (GTK_CONTAINER (scrolled_window), view);
  gtk_grid_attach (GTK_GRID (grid), scrolled_window, 12, 9, 4, 3);

    //-----------------------------------------//

  scrolled_window = gtk_scrolled_window_new (NULL, NULL);

  gtk_widget_set_hexpand (scrolled_window, FALSE);
  gtk_widget_set_vexpand (scrolled_window, FALSE);

  bufferConsole = gtk_entry_buffer_new ("Consola", 25);

  view = gtk_text_view_new ();
  //view = gtk_text_view_new_with_buffer (GTK_TEXT_BUFFER(bufferConsole));  // <- CORRIGIR
  gtk_text_view_set_editable (GTK_TEXT_VIEW (view), FALSE);
  gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (view), FALSE);
  gtk_container_add (GTK_CONTAINER (scrolled_window), view);
  gtk_grid_attach (GTK_GRID (grid), scrolled_window, 12, 5, 4, 4);
}

//-----------------------------------------------------------------------------//

void remLinha(char *i, GtkListStore* a) {
  GtkTreeIter iter;
  GtkTreePath *path;
  path = gtk_tree_path_new_from_string (i);
  gtk_tree_model_get_iter(GTK_TREE_MODEL(a), &iter, path);
  gtk_list_store_remove (GTK_LIST_STORE(a), &iter);
}

  //-----------------------------------------//


void insCode(char *line) {
    GtkTreeIter iter;
    enum stack {Index, Instruction, ValueA, TypeA, ValueB, TypeB, NUM_COLS };
    char* arr[NUM_COLS]; 
    int codePC;
    char signal;

    initLine(arr, NUM_COLS);
    sscanf(line, "CODE %c %s %s %s %s %s %s %d\n", &signal, arr[0], arr[1], arr[2], arr[3], arr[4], arr[5], &codePC);
    if(signal == '+'){
        gtk_list_store_append(storeCode, &iter);
        gtk_list_store_set (storeCode, &iter,   Index,          arr[0], 
                                                Instruction,    arr[1],
                                                ValueA,         arr[2],             
                                                TypeA,          arr[3],   
                                                ValueB,         arr[4],    
                                                TypeB,          arr[5],    
                                                -1);
    }
    actLabel(PC, codePC);
    freeLine(arr, NUM_COLS);
}

void insOP(char *line) {
    GtkTreeIter iter;
    enum stack {Index, Value, Type, NUM_COLS };
    char* arr[NUM_COLS];
    int sp, fp, gp;
    char signal;

    initLine(arr, NUM_COLS);
    sscanf(line, "OPSTACK %c %s %s %s %d %d %d\n", &signal, arr[0], arr[1], arr[2], &sp, &fp, &gp);
    if(signal == '+'){
        gtk_list_store_append(storeOP, &iter);
        gtk_list_store_set (storeOP, &iter, Index, arr[0], 
                                            Value, arr[1], 
                                            Type,  arr[2], 
                                            -1);
    }
    else if(signal == '-') gtk_list_store_remove (storeOP, &iter);
    actLabel(SP, sp);
    actLabel(FP, fp);
    actLabel(GP, gp);
    freeLine(arr, NUM_COLS);
}

void insHeap(char *line) {
  /*
    GtkTreeIter iter;
    enum stack {Index, Value, Type, NUM_COLS };
    gtk_list_store_append(storeHeap, &iter);
    gtk_list_store_set (storeHeap, &iter, Index, idx, Value, val, Type, tp, -1);
    */
}

void insCall(char *line) {
    GtkTreeIter iter;
    enum stack {Index, PcValue, FpValue , NUM_COLS };
    char* arr[NUM_COLS];
    int pc, fp;
    char signal;

    initLine(arr, NUM_COLS);
    sscanf(line, "CALLSTACK %c %s %s %s", &signal, arr[0], arr[1], arr[2]);
    if(signal == '+'){
        gtk_list_store_append(storeCall, &iter);
        gtk_list_store_set (storeCall, &iter, Index,    arr[0], 
                                              PcValue,  arr[1], 
                                              FpValue,  arr[2],
                                              -1);
    }
    else if(signal == '-') gtk_list_store_remove(storeCall, &iter);
    freeLine(arr, NUM_COLS);
}

//-----------------------------------------------------------------------------//

static void criarJanela (GtkWidget *view, int x, int y, int xx, int yy) {

  GtkWidget *scrolled_window;
  scrolled_window = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_set_hexpand (scrolled_window, TRUE);
  gtk_widget_set_vexpand (scrolled_window, TRUE);
  gtk_container_add (GTK_CONTAINER (scrolled_window), view);
  gtk_grid_attach (GTK_GRID (grid), scrolled_window, x, y, xx, yy);
}

//-----------------------------------------------------------------------------//

static void activateSCode (int x, int y, int xx, int yy) {
  GtkCellRenderer *renderer;
  GtkTreeModel    *model;

  viewC = gtk_tree_view_new ();
  renderer = gtk_cell_renderer_text_new ();\

  enum stack {Index, Instruction, ValueA, TypeA, ValueB, TypeB, NUM_COLS };
  static const char *nomes[] = {"Index", "Instruction", "ValueA", "TypeA", "ValueB", "TypeB"};
  storeCode = gtk_list_store_new (NUM_COLS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

  for (int coluna = Index; coluna < NUM_COLS; coluna++) {
    gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (viewC), -1, nomes[coluna], renderer, "text", coluna, NULL);
  }

  model = GTK_TREE_MODEL (storeCode);
  gtk_tree_view_set_model (GTK_TREE_VIEW (viewC), model);

  criarJanela(viewC, x, y, xx, yy);
}

  //-----------------------------------------//

static void activateSHeap (int x, int y, int xx, int yy) {

  GtkWidget       *view;
  GtkCellRenderer *renderer;
  GtkTreeModel    *model;

  view = gtk_tree_view_new ();
  renderer = gtk_cell_renderer_text_new ();\

  enum stack {Index, Value, Type, NUM_COLS };
  static const char *nomes[] = {"Index", "Value", "Type"};
  storeHeap = gtk_list_store_new (NUM_COLS, G_TYPE_INT, G_TYPE_INT, G_TYPE_STRING);

  for (int coluna = Index; coluna < NUM_COLS; coluna++) {
    gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view), -1, nomes[coluna], renderer, "text", coluna, NULL);
  }

  model = GTK_TREE_MODEL (storeHeap);
  gtk_tree_view_set_model (GTK_TREE_VIEW (view), model);

  criarJanela(view, x, y, xx, yy);
}

  //-----------------------------------------//

static void activateSOP (int x, int y, int xx, int yy) {

  GtkWidget       *view;
  GtkCellRenderer *renderer;
  GtkTreeModel    *model;

  view = gtk_tree_view_new ();
  renderer = gtk_cell_renderer_text_new ();\

  enum stack {Index, PcValue, FpValue, NUM_COLS };
  static const char *nomes[] = {"Index", "Value", "Type"};
  storeOP = gtk_list_store_new (NUM_COLS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

  for (int coluna = Index; coluna < NUM_COLS; coluna++) {
    gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view), -1, nomes[coluna], renderer, "text", coluna, NULL);
  }

  model = GTK_TREE_MODEL (storeOP);
  gtk_tree_view_set_model (GTK_TREE_VIEW (view), model);

  criarJanela(view, x, y, xx, yy);
}

    //-----------------------------------------//

static void activateSCall (int x, int y, int xx, int yy) {

  GtkWidget       *view;
  GtkCellRenderer *renderer;
  GtkTreeModel    *model;

  view = gtk_tree_view_new ();
  renderer = gtk_cell_renderer_text_new ();

  enum stack {Index, Value, Type, NUM_COLS };
  static const char *nomes[] = {"Index", "PcValue", "FpValue"};
  storeCall= gtk_list_store_new (NUM_COLS, G_TYPE_INT, G_TYPE_INT, G_TYPE_STRING);

  for (int coluna = Index; coluna < NUM_COLS; coluna++) {
    gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view), -1, nomes[coluna], renderer, "text", coluna, NULL);
  }

  model = GTK_TREE_MODEL (storeCall);
  gtk_tree_view_set_model (GTK_TREE_VIEW (view), model);

  criarJanela(view, x, y, xx, yy);
}

//-----------------------------------------------------------------------------//

void activateStacks () {
  activateSCode (0, 1, 3, 11);
  activateSHeap (3, 1, 3, 11);
  activateSOP   (6, 1, 3, 11);
  activateSCall (9, 1, 3, 11);

  GtkTreeSelection x;

}

//-----------------------------------------------------------------------------//

static void activate () {
    grid = gtk_grid_new ();
    gtk_container_add (GTK_CONTAINER (window), grid);
    gtk_grid_set_row_spacing (GTK_GRID (grid),10);
    gtk_grid_set_column_spacing (GTK_GRID (grid),10);
    //gtk_grid_set_column_homogeneous (GTK_GRID (grid),TRUE);

    activateInputs ();
    activateStacks ();
    activateLables ();

    gtk_widget_show_all (window);
}

//-----------------------------------------------------------------------------//



int main (int argc, char **argv) {
    gtk_init (&argc, &argv);

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 400);
    gtk_window_set_title (GTK_WINDOW (window), "VMS-Projeto");
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);
    
    g_signal_connect (window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    activate();

    gtk_widget_show (window);
    gtk_main ();
    return 0;
}
