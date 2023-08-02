/**
 * @file addbook.c
 * @author Sohaib Mohamed (sohaib.amhmd@gmail.com)
 * @brief addbook.c file
 * @version 0.1
 * @date 2023-08-02
 * @page this file is addbook.c
 *
 * @copyright Copyright (c) 2023
 * Released under the GNU GPLv2+,
 * Everyone is permitted to copy and distribute verbatim copies
 * of this license document, but changing it is not allowed.
 *
 */
#include <sqlite3.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fort.h" // Pretty Print Tables
#include "log.h"

#include "addbook.h"


static int row_counter;

void adbk_welcome_mgs(void) {
   adbk_print(
      "             _     _ ____              _    \n"
      "    /\\      | |   | |  _ \\            | |   \n"
      "   /  \\   __| | __| | |_) | ___   ___ | | __\n"
      "  / /\\ \\ / _` |/ _` |  _ < / _ \\ / _ \\| |/ /\n"
      " / ____ \\ (_| | (_| | |_) | (_) | (_) |   < \n"
      "/_/    \\_\\__,_|\\__,_|____/ \\___/ \\___/|_|\\_\\"
      "                                      \n"
      "Address Book System...\n"
      "\n"
   );
}

void adbk_help_mgs(void) {
   adbk_print(
      "(a) Add new record\n"
      "(d) Delete a record\n"
      "(h) Help. prints this message\n"
      "(l) List all record\n"
      "(m) Modify a record\n"
      "(s) Search by name\n"
   );
   log_info("%s() function called", __func__);
}

sqlite3* adbk_db_connect(char* db_name) {
   sqlite3* db;
   char* err_msg = 0;
   int rc = sqlite3_open(db_name, &db);

   log_info("%s() function called", __func__);

   if (rc != SQLITE_OK) {
      log_error("SQL error: %s: %s\n", __func__, sqlite3_errmsg(db));

      sqlite3_free(err_msg);
      sqlite3_close(db);
      return NULL;
   }

   log_info("DB connection successfully opened");

   return db;
}

void adbk_db_disconnect(sqlite3* db) {
   log_info("%s() function called", __func__);
   sqlite3_close(db);
}

// Tolerant function, creates Record table if not already exit.
int adbk_db_record_new(sqlite3* db) {
   char* err_msg = 0;
   int rc;
   char* sql =
      "CREATE TABLE IF NOT EXISTS Record(Id INTEGER PRIMARY KEY AUTOINCREMENT, FName TEXT, LName TEXT, Email TEXT, Phone varchar(15), Street INT, Town TEXT, Country TEXT);";

   log_info("%s() function called", __func__);

   rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

   if (rc != SQLITE_OK ) {
      log_error("SQL error: %s: %s\n", __func__, err_msg);

      sqlite3_free(err_msg);
      sqlite3_close(db);
      return 1;
   }

   return 0;
}

int adbk_db_record_add(sqlite3* db, Record* record) {
   char* err_msg = 0;
   int rc;
   char sql[500];

   log_info("%s() function called", __func__);

   sprintf(sql, "INSERT INTO Record (FName, LName, Email, Phone, Street, Town, Country) VALUES('%s', '%s', '%s', '%s', %d, '%s', '%s');",
           record->f_name,
           record->l_name,
           record->email,
           record->phone,
           record->street,
           record->town,
           record->country);

   rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

   // Print the last Id added to DB
   int last_id = sqlite3_last_insert_rowid(db);
   log_info("The last Id of the inserted row is %d\n", last_id);

   /* Print one row table */
   adbk_print_table_onerow(last_id, record);

   if (rc != SQLITE_OK ) {
      log_error("SQL error: %s: %s\n", __func__, err_msg);

      sqlite3_free(err_msg);
      return 1;
   }

   return 0;
}

int adbk_db_record_remove(sqlite3* db, int id) {
   char* err_msg = 0;
   int rc;
   char sql[500];

   log_info("%s() function called", __func__);

   sprintf(sql, "DELETE FROM Record WHERE id=%d;", id);

   rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

   if (rc != SQLITE_OK ) {
      log_error("SQL error: %s: %s\n", __func__, err_msg);

      sqlite3_free(err_msg);
      return 1;
   }

   return 0;
}

static int select_all_callback(void* userdata, int argc, char** argv, char** col_name) {
   // Retrieve the user data pointer
   Record* data = (Record*)userdata;

   for (int i = 0; i < argc; i++) {
      if ((strcmp(col_name[i], "FName") == 0)) {
         strcpy(data[row_counter].f_name, argv[i]);
      } else if ((strcmp(col_name[i], "LName") == 0)) {
         strcpy(data[row_counter].l_name, argv[i]);
      } else if ((strcmp(col_name[i], "Email") == 0)) {
         strcpy(data[row_counter].email, argv[i]);
      } else if ((strcmp(col_name[i], "Phone") == 0)) {
         strcpy(data[row_counter].phone, argv[i]);
      } else if ((strcmp(col_name[i], "Street") == 0)) {
         strcpy(data[row_counter].street_buff, argv[i]);
      } else if ((strcmp(col_name[i], "Town") == 0)) {
         strcpy(data[row_counter].town, argv[i]);
      } else if ((strcmp(col_name[i], "Country") == 0)) {
         strcpy(data[row_counter].country, argv[i]);
      } else if ((strcmp(col_name[i], "Id") == 0)) {
         strcpy(data[row_counter].id_buff, argv[i]);
      }
   }
   row_counter++;

   return 0;
}

static int adbk_db_record_row_count(sqlite3* db) {
   char* err_msg = 0;
   int rc;
   sqlite3_stmt* stmt;
   char* sql = "select count(*) from Record";

   log_info("%s() function called", __func__);

   rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

   if (rc != SQLITE_OK ) {
      log_error("SQL error: %s: %s\n", __func__, err_msg);

      sqlite3_free(err_msg);
      return 1;
   }

   rc = sqlite3_step(stmt);
   if (rc != SQLITE_ROW) {
      // error handling -> no rows returned, or an error occurred
   }
   return sqlite3_column_int(stmt, 0);
}

int adbk_db_record_select_all(sqlite3* db) {
   char* err_msg = 0;
   int rc;
   char* sql = "SELECT * FROM Record";
   row_counter = 0;

   /* get the total number of rows in DB */
   printf("%d\n", adbk_db_record_row_count(db));
   int total_rows = adbk_db_record_row_count(db);

   // Alloc records FIXME free it
   Record* records = malloc(total_rows * sizeof(Record));

   log_info("%s() function called", __func__);

   rc = sqlite3_exec(db, sql, select_all_callback, records, &err_msg);

   if (rc != SQLITE_OK ) {
      log_error("SQL error: %s: %s\n", __func__, err_msg);

      sqlite3_free(err_msg);
      return 1;
   }

   // Print Table
   adbk_print_table(total_rows, records);

   return 0;
}

static int search_callback(void* NotUsed, int argc, char** argv, char** azColName) {

   for (int i = 0; i < argc; i++) {
      adbk_print("%s = %s\t ", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   adbk_print("\n");

   return 0;
}

int adbk_db_record_search(sqlite3* db, char* keyword) {
   char* err_msg = 0;
   int rc;
   char sql[250];

   log_info("%s() function called", __func__);

   sprintf(sql, "SELECT * FROM Record WHERE FName LIKE '\%%%s\%%'"
           "OR LName LIKE '%%%s%%'"
           "OR Email LIKE '%%%s%%'"
           "OR Phone LIKE '%%%s%%'"
           "OR Street LIKE '%%%s%%'"
           "OR TOWN LIKE '%%%s%%'"
           "OR Country LIKE '%%%s%%';",
           keyword,
           keyword,
           keyword,
           keyword,
           keyword,
           keyword,
           keyword);

   rc = sqlite3_exec(db, sql, search_callback, 0, &err_msg);

   if (rc != SQLITE_OK ) {
      log_error("SQL error: %s: %s\n", __func__, err_msg);

      sqlite3_free(err_msg);
      return 1;
   }

   return 0;
}

// catch the new values from user and send it back to adbk_db_record_modify via userdata
static int modify_view_callback(void* userdata, int argc, char** argv, char** col_name) {
   // Retrieve the user data pointer
   Record* data = (Record*)userdata;

   // TODO accept empty inputs... to skip
   for (int i = 0; i < argc; i++) {
      int ok;
      adbk_print("%s (%s):\n", col_name[i], argv[i] ? argv[i] : "NULL");

      if ((strcmp(col_name[i], "FName") == 0)) {
         ok = scanf("%s", data->f_name);
         if (!ok) {
            log_error("%s: Invalid input!", __func__);
            return 1;
         }
      } else if ((strcmp(col_name[i], "LName") == 0)) {
         ok = scanf(" %s", data->l_name);
         if (!ok) {
            log_error("%s: Invalid input!", __func__);
            return 1;
         }
      } else if ((strcmp(col_name[i], "Email") == 0)) {
         ok = scanf(" %s", data->email);
         if (!ok) {
            log_error("%s: Invalid input!", __func__);
            return 1;
         }
      } else if ((strcmp(col_name[i], "Phone") == 0)) {
         ok = scanf(" %s", data->phone);
         if (!ok) {
            log_error("%s: Invalid input!", __func__);
            return 1;
         }
      } else if ((strcmp(col_name[i], "Street") == 0)) {
         ok = scanf(" %d", &data->street);
         if (!ok) {
            log_error("%s: Invalid input!", __func__);
            return 1;
         }
      } else if ((strcmp(col_name[i], "Town") == 0)) {
         ok = scanf(" %s", data->town);
         if (!ok) {
            log_error("%s: Invalid input!", __func__);
            return 1;
         }
      } else if ((strcmp(col_name[i], "Country") == 0)) {
         ok = scanf(" %s", data->country);
         if (!ok) {
            log_error("%s: Invalid input!", __func__);
            return 1;
         }
      }
   }

   return 0;
}

int adbk_db_record_modify(sqlite3* db, int id) {
   char* err_msg = 0;
   int rc;
   char sql[1000];
   Record record;

   log_info("%s() function called", __func__);

   sprintf(sql, "SELECT * FROM Record WHERE id = %d;", id);

   rc = sqlite3_exec(db, sql, modify_view_callback, &record, &err_msg);

   if (rc != SQLITE_OK ) {
      log_error("SQL error: %s: %s\n", __func__, err_msg);

      sqlite3_free(err_msg);

      return 1;
   }

   sprintf(sql, "UPDATE Record SET FName='%s', LName='%s', Email='%s', Phone='%s', Street=%d, Town='%s', Country='%s' WHERE id=%d;",
           record.f_name,
           record.l_name,
           record.email,
           record.phone,
           record.street,
           record.town,
           record.country,
           id);

   // Print the last Id added to DB
   int last_id = sqlite3_last_insert_rowid(db);

   /* Print one row table */
   adbk_print_table_onerow(last_id, &record);

   rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

   if (rc != SQLITE_OK ) {
      log_error("SQL error: %s: %s\n", __func__, err_msg);

      sqlite3_free(err_msg);

      return 1;
   }

   return 0;
}

// use adbk_print for consistance instead of printf
void adbk_print(const char* format, ...) {
   va_list args;
   va_start(args, format);

   vprintf(format, args);

   va_end(args);
}

void adbk_print_options_menu(void) {
   adbk_print("Hit (a/c/d/h/l/m/s), h for help or q for exit: ");
}

void adbk_print_table_onerow(int id, Record* record) {
   char street_buff[100];
   char id_buff[100];

   log_info("%s() function called", __func__);

   ft_table_t* table = ft_create_table();
   ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
   ft_write_ln(table, "ID", "FName", "LName", "Email", "Phone", "Street", "Town", "Country");

   sprintf(street_buff, "%d", record->street);
   sprintf(id_buff, "%d", id);

   ft_write_ln(table,
               id_buff,
               record->f_name,
               record->l_name,
               record->email,
               record->phone,
               street_buff,
               record->town,
               record->country);

   printf("%s\n", ft_to_string(table));
   ft_destroy_table(table);
}

void adbk_print_table(int total_rows, Record* records) {
   log_info("%s() function called", __func__);

   ft_table_t* table = ft_create_table();
   ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
   ft_write_ln(table, "ID", "FName", "LName", "Email", "Phone", "Street", "Town", "Country");

   for (size_t i = 0; i < total_rows; i++) {
      ft_write_ln(table,
                  records[i].id_buff,
                  records[i].f_name,
                  records[i].l_name,
                  records[i].email,
                  records[i].phone,
                  records[i].street_buff,
                  records[i].town,
                  records[i].country);
   }
   printf("%s\n", ft_to_string(table));
   ft_destroy_table(table);
}
