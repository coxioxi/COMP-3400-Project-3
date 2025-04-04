#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>

int
main ()
{
  // Starter HTML (to avoid copy-and-paste annoynances):
  printf ("<html lang=\"en\">\n");
  printf ("  <head>\n");
  printf ("    <title>File Hash Database</title>\n");
  printf ("    <link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn."
          "com/bootstrap/4.3.1/css/bootstrap.min.css\" integrity=\""
          "sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/"
          "iJTQUOhcWr7x9JvoRxT2MZw1T\" crossorigin=\"anonymous\" />\n");
  printf ("  </head>\n\n");
  
  // These variables can be read from the environment using getenv().
  char *db = getenv("db");
  char *record = getenv("record");
  char *hash = getenv("hash");
  char *query = getenv("QUERY_STRING");
  // This is an HTML comment. It's useful for debugging to see if your
  // environment variables got through.
  printf ("  <!-- Environment variables:\n");
  printf ("       db: %s\n", db);
  printf ("       record: %s\n", record);
  printf ("       hash: %s\n", hash);
  printf ("       query: %s\n", query);
  printf ("    -->\n\n");

  // TODO [PART]: Read the data/data.txt file and produce an HTML table
  // to match the output in the cgi-src/tests/expected/PART_show_all.txt
  // file. Note that each line in the data.txt file should correspond to
  // two "col" divs, and all but the last line should be followed by a
  // "w-100" div. The overall framework of the body should look like this
  // (with exactly 2 spaces before the opening < character of each line):
  //    <body>
  //      <div class="container">
  //        <br />
  //        <h2 class="mb-0">Database Records</h2>
  //        <div class="row">
  //          <div class="col py-md-2 border bg-light">PART_hello.txt</div>
  //          <div class="col py-md-2 border bg-light">94079f...</div>
  //          <div class="w-100"></div>
  //          <div class="col py-md-2 border bg-light">PART_show_all.txt</div>
  //          <div class="col py-md-2 border bg-light">9e5543...</div>
  //        </div>
  //      </div>
  //    </body>
    
	printf("  <body>\n");
  printf("    <div class=\"container\">\n");
  printf("      <br />\n");
  printf("      <h2 class=\"mb-0\">Database Records</h2>\n");
  printf("      <div class=\"row\">\n");
  
  if(query != NULL) // Query is set
  {
  	// If QUERY_STRING is set, use that and split it apart at the & character
  	char *token;
  	const char *delim = "=&";

		token = strtok(query, delim);
		
		while(token != NULL)
		{
			if(strcmp(token, "db") == 0)
			{
				db = strtok(NULL, delim);
			}
			else if(strcmp(token, "record") == 0)
			{
				record = strtok(NULL, delim);
			}
			else if(strcmp(token, "hash") == 0)
			{
				hash = strtok(NULL, delim);
			}
			
			token = strtok(NULL, delim);
		}
  }
  
  if(db == NULL)
  {
  	db = "data.txt";
  }

	char realdb[256] = "data/";
	strcat(realdb, db);
	
  FILE *file = fopen(realdb, "r");
	char filename [1024];
  char nums [1024];
  int i = 0;
 
  int recnum = -1;
  
  if(record != NULL)
  {
  	recnum = atoi(record);
  }
 
	while(fscanf(file, "%s", nums) == 1)
	{
			fscanf(file, "%s", filename);
			
			if(i != 0 && recnum == -1)
      	printf("        <div class=\"w-100\"></div>\n");
      
      if(i+1 == recnum || recnum == -1)
      {	
				printf("        <div class=\"col py-md-2 border bg-light\">%s</div>\n", filename);
				
				if(recnum != -1 && recnum == i+1 && hash != NULL && strcmp(hash, nums) != 0)
					printf("        <div class=\"col py-md-2 border bg-light\">%s <span class=\"badge badge-danger\">MISMATCH</span></div>\n", nums);
		    else
		    	printf("        <div class=\"col py-md-2 border bg-light\">%s</div>\n", nums);
      }
      
      ++i;
	}
	
	printf("      </div>\n");
  printf("    </div>\n");
  printf("  </body>\n");
	
	
  fclose(file);

  // TODO [MIN]: Once you have the basic structure working, extend it to
  // read in environment variables (db, record, hash, and QUERY_STRING).
  // From a logic standpoint, if QUERY_STRING is set, use that and split it
  // apart at the & character. For example, the QUERY_STRING might look like:
  //   db=foo.txt&record=2&hash=9e5543354d4592db8272b3c3e14953770df88ba3
  // If the QUERY_STRING is not set, look for the db, record, and hash
  // environment variables independently. If the hash variable is set,
  // compare its value with the hash value for the specified record. (If
  // record is not set, then ignore the hash variable.) If the hash does not
  // match, add this code just after the hash value from the database (put
  // a space before the <span and no space between </span></div>):
  //    <span class="badge badge-danger">MISMATCH</span>
 
  printf ("\n</html>\n");

  return 0;
}
