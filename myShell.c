#include "myShell.h"
#include "myFunction.h"

int main()
{
    system("clear");
    for (int i = 0; i < 100; i++)
    {
        welcome();
        system("clear");
        
        welcome1();
    }
    while (1)
    {
        getLocation();
        char *input = getInputFromUser();
        // cp\0<file>\0<file>\0
        // cp <file> <file>\0
        // [cp, <file>, <file> ]
        // [input,input+3,input+10]

        char **arg = splitArgument(input);
        int i = 0;
        while (*(arg + i) != NULL)
        {
            puts(arg[i]);
            i++;
        }

        if (strcmp(input, "exit") == 0)
        {
            free(arg);
            free(input);
            puts("log out");
            break;
        }

        free(arg);
        free(input);
    }
    return 0;
}
// יש לכתוב את פונקציית הברוכים הבאים כרצונכם אבל קצת יותר ממה שמוצג מטה לדוגמא:
//                     aSPY//YASa
//              apyyyyCY//////////YCa       |
//             sY//////YSpcs  scpCY//Pp     | Welcome to myShell
//  ayp ayyyyyyySCP//Pp           syY//C    | Version 2.4.3
//  AYAsAYYYYYYYY///Ps              cY//S   |
//          pCCCCY//p          cSSps y//Y   | https://github.com/<user>
//          SPPPP///a          pP///AC//Y   |
//               A//A            cyP////C   | Have fun!
//               p///Ac            sC///a   |
//               P////YCpc           A//A   | Craft packets like it is your last
//        scccccp///pSP///p          p//Y   | day on earth.
//       sY/////////y  caa           S//P   |                      -- Lao-Tze
//        cayCyayP//Ya              pY/Ya   |
//         sY/PsY////YCc          aC//Yp
//          sc  sccaCY//PCypaapyCP//YSs
//                   spCPY//////YPSps
//                        ccaacs
//                                        using c                                    using c
void welcome()
{
    printf("           ,--.\n");
    printf("    ,--.   |  |\n");
    printf("   |oo  | _\\  | .-.\n");
    printf("   |~~  |/  \\ |' | \\__\n");
    printf("   |/\\/\\|  /  \\|  |/  /\n");
    printf("      \\ \\,/;      \\  /\n");
    printf("       \\/  \\      | |\n");
    printf("        |   \\     | |\n");
    printf("       /    |    /  /\n");
    printf("      |,/\\  |___|__/\n");
    printf("      \\ | \\ |   | |\n");
    printf("       ||\\|     | |\n");
    printf("       ||||    |  |\n");
    printf("       \\||    /  \\\n");
    printf("        `|   |    |\n");
    printf("         |  |    |\n");
    printf("        /|  |    |\n");
    printf("      /  |  |    |\n");
    printf("    /    |  |    |\n");
    printf("   |     |  |    |\n");
    printf("\n");
}
void welcome1()
{
    printf("           ,--.\n");
    printf("    ,--.   |  |\n");
    printf("   |--  | _\\  | .-.\n");
    printf("   |~~  |/  \\ |' | \\__\n");
    printf("   |/\\/\\|  /  \\|  |/  /\n");
    printf("      \\ \\,/;      \\  /\n");
    printf("       \\/  \\      | |\n");
    printf("        |   \\     | |\n");
    printf("       /    |    /  /\n");
    printf("      |,/\\  |___|__/\n");
    printf("      \\ | \\ |   | |\n");
    printf("       ||\\|     | |\n");
    printf("       ||||    |  |\n");
    printf("       \\||    /  \\\n");
    printf("        `|   |    |\n");
    printf("         |  |    |\n");
    printf("        /|  |    |\n");
    printf("      /  |  |    |\n");
    printf("    /    |  |    |\n");
    printf("   |     |  |    |\n");
    printf("\n");
}
