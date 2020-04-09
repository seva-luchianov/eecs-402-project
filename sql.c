#include <my_global.h>
#include <mysql.h>
#include <string.h>

void finish_with_error(MYSQL *con)
{
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

int main(int argc, char **argv)
{
    printf("MySQL client version: %s\n", mysql_get_client_info());
    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }  

    if (mysql_real_connect(
        con, "localhost", "root", "root", 
        "eecs402proj", 0, NULL, 0
    ) == NULL)
    {
        finish_with_error(con);
    }

    char q[1000] = "SELECT * FROM ipmap WHERE ip='";
    strcat(q, "127.0.0.1");
    strcat(q, "'");
    printf("Query: %s\n", q);

    if (mysql_query(con, q))
    {
        finish_with_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) 
    {
        finish_with_error(con);
    }

    int num_fields = mysql_num_fields(result);

    MYSQL_ROW row;

    char *matchingUUID = NULL;
    matchingUUID = (char*)calloc(500 + 1, sizeof(char));

    int foundUUID = 0;

    while ((row = mysql_fetch_row(result))) 
    {
        foundUUID = 1;
        for(int i = 0; i < num_fields; i++) 
        {
            printf("%s ", row[i] ? row[i] : "NULL"); 
        }
        printf("\n");
        matchingUUID = row[1];
    }

    if (foundUUID == 1)
    {
        printf("Found UUID: %s\n", matchingUUID);
    }
    else
    {
        printf("No UUID found\n");
    }

    mysql_free_result(result);
    mysql_close(con);

    exit(0);
}
