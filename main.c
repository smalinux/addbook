/**
 * @file main.c
 * @author Sohaib Mohamed (sohaib.amhmd@gmail.com)
 * @brief Main.c file
 * @version 0.1
 * @date 2023-08-02
 * @page this file is main.c
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


static struct config {
   char db_name[10];  /* DB Name */
   bool db_connected; /* Keep track the status of conn open/close? */
} config = {
   .db_name = "addbook.db",
   .db_connected = 0,
};

int main(int argc, char** argv) { // TODO argc & argv support
   char input;
   sqlite3* db;
   Record record;
   int ok;

   adbk_welcome_mgs();
   adbk_help_mgs();

   // Open Connect db
   db = adbk_db_connect(config.db_name);
   log_info("DB conection opened");

   // Create DB Table: Record
   adbk_db_record_new(db);
   log_info("DB Table Record is ready");

   adbk_print_options_menu();
   ok = scanf(" %c", &input);
   if (!ok) {
      log_error("%s: Invalid input!", __func__);
   }

   /*
    * The main loop
    */
   while (input != 'q') {
      /*
       * (a) Add new Record
       */
      if (input == 'a') {
         adbk_print("> Add new record...\n");

         // First Name
         adbk_print("> First Name: ");
         ok = scanf(" %50[^\n]", record.f_name);
         if (!ok) {
            log_error("%s: Invalid input!", __func__);
            continue;
         }

         // Last Name
         adbk_print("> Last Name: ");
         ok = scanf(" %50[^\n]", record.l_name);
         if (!ok) {
            log_error("%s: Invalid input!", __func__);
            continue;
         }

         // Email Address
         // TODO Add Email Validation
         adbk_print("> Email Address (max 100 char): ");
         ok = scanf(" %100[^\n]", record.email);
         if (!ok) {
            log_error("%s: Invalid input!", __func__);
            continue;
         }

         // Telephone number
         adbk_print("> Phone: ");
         // TODO code validation for logical phone inputs (Leave it now)
         ok = scanf(" %s", record.phone);
         if (!ok) {
            log_error("%s: Invalid input!", __func__);
            continue;
         }

         adbk_print("Address. \n");

         // Street
         adbk_print("> Street: ");
         ok = scanf(" %d", &record.street);

         // Town
         adbk_print("> Town (max 50 char): ");
         ok = scanf(" %100[^\n]", record.town);
         if (!ok) {
            log_error("%s: Invalid input!", __func__);
            continue;
         }

         // Country
         adbk_print("> Country (max 50 char): ");
         ok = scanf(" %100[^\n]", record.country);
         if (!ok) {
            log_error("%s: Invalid input!", __func__);
            continue;
         }

         adbk_db_record_add(db, &record);

         /*
          * (d) Delete a record
          */
      } else if (input == 'd') {
         int id = 0;
         adbk_print("> Delete record... id: ");
         ok = scanf(" %d", &id);
         if (!ok) {
            log_error("%s: Invalid input!", __func__);
            continue;
         }

         adbk_db_record_remove(db, id);

         // TODO Print pretty table in terminal (add pretty table func)
         // and Sure? message (Y/n)

         /*
          * (h) help message
          */
      } else if (input == 'h') { // 'h' key
         adbk_help_mgs();

         /*
          * (l) List all records
          */
      } else if (input == 'l') { // 'l' key
         adbk_db_record_select_all(db);
         // TODO Print pretty table in terminal (add pretty table func)
         // and Sure? (Y/n)

         /*
          * (m) Modify a record
          */
      } else if (input == 'm') { // 'm' key
         int id = 0;
         adbk_print("> Modify record... id: ");
         ok = scanf(" %d", &id);
         if (!ok) {
            log_error("%s: Invalid input!", __func__);
            continue;
         }

         adbk_db_record_modify(db, id);

         /*
          * (s) Search by name
          */
      } else if (input == 's') { // 's' key
         adbk_print("> Search by: ");
         char keyword[20];
         ok = scanf(" %s", keyword);
         if (!ok) {
            log_error("%s: Invalid input!", __func__);
            continue;
         }

         adbk_db_record_search(db, keyword);

         /*
          * Command not found
          */
      } else {
         adbk_print("Option (%c) not exist!\n", input);
         adbk_help_mgs();
      }

      adbk_print_options_menu();
      ok = scanf(" %c", &input);
      if (!ok) {
         log_error("%s: Invalid input!", __func__);
         continue;
      }
   }

   // Close DB Connect
   adbk_db_disconnect(db);
   log_info("DB conection closed");

   return 0;
}
