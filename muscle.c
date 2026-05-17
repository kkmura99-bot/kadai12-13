#include <stdio.h>
#include <string.h>
#include <math.h>
#define Epley(w, r) ((w) * (1 + (r) / 30.0))
#define LOMBARDI(w, r) ((w) * pow((r), 0.10))
#define WATHAN(w, r) (100.0 * (w) / (48.8 + 53.8 * exp(-0.075 * (r))))

double weight = 0.0;
int reps, sets;
int name;
char date[15];
enum select
{
    bench,
    legg,
    abdmi,
    dunb,
    smith
};
int i;

// 構造体
typedef struct
{
    char date[20];
    int exercise;
    double weight;
    int reps;
    double rm;
} Record;

Record recs[365];
int rec_count = 0;
/* RMを計算し、セットごとの最大RMを記録する関数 */
void RM(void)
{
    double max_rm = 0.0;
    // 種目選択

    while (1)
    {
        printf("種目名を選んでください(元の画面:5)\n");
        printf("0: ベンチプレス\n");
        printf("1: レッグプレス\n");
        printf("2: アブドミナル\n");
        printf("3: ダンベルカール\n");
        printf("4: スミスマシン\n");
        printf("番号を選択 >> ");

        if (scanf("%d", &name) != 1)
        {
            printf("不正な入力です（数字を入力してください）\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        if (name == 5)
        {
            printf("メニューに戻ります\n");
            return;
        }
        if (name < 0 || name > 4)
        {
            printf("範囲外の数字です\n");
            continue;
        }

        break;
    }

    switch (name)
    {
    case bench:
        printf("ベンチプレス\n");
        break;
    case legg:
        printf("レッグプレス\n");
        break;
    case abdmi:
        printf("アブドミナル\n");
        break;
    case dunb:
        printf("ダンベルカール\n");
        break;
    case smith:
        printf("スミスマシン\n");
        break;
    default:
        printf("不正な入力\n");
        return;
    }
    int y, m, d;

    printf("日付を入力してください (例: 2025-12-11): ");
    if (scanf("%d-%d-%d", &y, &m, &d) != 3)
    {
        printf("日付の形式が正しくありません\n");
        while (getchar() != '\n')
            ;
        return;
    }

    if (m < 1 || m > 12 || d < 1 || d > 31)
    {
        printf("存在しない日付です\n");
        return;
    }

    sprintf(date, "%04d-%02d-%02d", y, m, d);

    printf("セット数を入力してください(戻る:0)");
    scanf("%d", &sets);
    if (sets <= 0)
    {
        printf("メニューに戻ります");
        return;
    }
    for (int i = 0; i < sets; i++)
    {
        printf("重量(kg)を入力してください: ");
        scanf("%lf", &weight);
        printf("回数を入力してください: ");
        scanf("%d", &reps);

        // RM計算(RMは筋トレの時に使う指標)
        double rm_e = Epley(weight, reps);
        double rm_l = LOMBARDI(weight, reps);
        double rm_w = WATHAN(weight, reps);

        printf("Epley   : %.2f\n", rm_e);
        printf("LOMBARDI : %.2f\n", rm_l);
        printf("WATHAN: %.2f\n", rm_w);

        double rm = rm_e;
        if (rm_w > rm)
            rm = rm_w;
        if (rm_l > rm)
            rm = rm_l;
        printf("このセットのRM = %.2f\n", rm);

        if (rm > max_rm)
        {
            max_rm = rm;
        }
    }
    printf("日付: %s\n", date);
    printf("\n=========================\n");
    printf("最大RM: %.2f\n", max_rm);
    printf("=========================\n");
    /* ---- 記録保存 ---- */
    if (rec_count < 365)
    {
        recs[rec_count].exercise = name;
        recs[rec_count].weight = weight;
        recs[rec_count].reps = reps;
        recs[rec_count].rm = max_rm;
        strcpy(recs[rec_count].date, date);
        rec_count++;
        printf("記録を保存しました。\n");
    }
}
/* 保存された記録を日付順に並べて一覧表示する関数 */
void show(void)
{
    if (rec_count == 0)
    {
        printf("記録がありません\n");
        return;
    }
    for (int i = 0; i < rec_count - 1; i++)
    {
        for (int j = i + 1; j < rec_count; j++)
        {
            if (strcmp(recs[i].date, recs[j].date) > 0)
            {
                Record tmp = recs[i];
                recs[i] = recs[j];
                recs[j] = tmp;
            }
        }
    }
    for (int i = 0; i < rec_count; i++)
    {
        const char *ename;
        if (recs[i].exercise == 0)
        {
            ename = "ベンチプレス";
        }
        else if (recs[i].exercise == 1)
        {
            ename = "レッグプレス";
        }
        else if (recs[i].exercise == 2)
        {
            ename = "アブドミナル";
        }
        else if (recs[i].exercise == 3)
        {
            ename = "ダンベルカール";
        }
        else if (recs[i].exercise == 4)
        {
            ename = "スミスマシン";
        }
        else
        {
            ename = "不明";
        }
        printf("%d) %s | %s | %.1fkg x %d回 | RM: %.2f\n",
               i + 1,
               recs[i].date,
               ename,
               recs[i].weight,
               recs[i].reps,
               recs[i].rm);
    }
}
// ファイル保存
void save(void)
{
    FILE *fp;
    if ((fp = fopen("training.txt", "w")) == NULL)
    {
        printf("\aファイルをオープンできません。\n");
    }
    else
    {
        printf("記録を呼び出します\n");
        for (int i = 0; i < rec_count; i++)
        {
            fprintf(fp, "%s %d %.1f %d %.2f\n",
                    recs[i].date,
                    recs[i].exercise,
                    recs[i].weight,
                    recs[i].reps,
                    recs[i].rm);
        }
        fclose(fp);
        printf("保存しました\n");
    }
}
/* 記録をtraining.txtに書き込んで保存する関数 */
void load(void)
{
    FILE *fp;
    if ((fp = fopen("training.txt", "r")) == NULL)
    {
        printf("\a保存ファイルがありません\n");
    }
    else
    {
        while (rec_count < 365 && fscanf(fp, "%s %d %lf %d %lf",
                                         recs[rec_count].date, &recs[rec_count].exercise, &recs[rec_count].weight, &recs[rec_count].reps, &recs[rec_count].rm) == 5)
        {
            rec_count++;
        }
        fclose(fp);
        printf("%dの記録を読み込みました\n", rec_count);
    }
}
/* 選択した種目のRM推移を*で可視化する関数 */
void graph(void)
{
    int ex;
    int found = 0;
    do
    {
        printf("種目を選択してください(戻る:5)\n");
        printf("0: ベンチプレス\n");
        printf("1: レッグプレス\n");
        printf("2: アブドミナル\n");
        printf("3: ダンベルカール\n");
        printf("4: スミスマシン\n");
        printf("番号 > ");
        if (scanf("%d", &ex) != 1)
        {
            printf("不正な入力です（数字を入力してください）\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        if (ex == 5)
        {
            printf("メニューに戻ります。\n");
            return;
        }
    } while (ex < 0 || ex > 5);
    printf("推移表示\n");

    for (int i = 0; i < rec_count; i++)
    {
        if (recs[i].exercise == ex)
        {
            int stars = (recs[i].rm / 2);

            printf("%s | %5.1f | ", recs[i].date, recs[i].rm);
            for (int j = 0; j < stars; j++)
            {
                printf("*");
            }
            printf("\n");

            found = 1;
        }
    }

    if (!found)
    {
        printf("この種目の記録はありません。\n");
    }
}
/* 指定した番号の記録を削除する関数 */
void delete(void)
{
    if (rec_count == 0)
    {
        printf("削除できる記録がありません。\n");
        return;
    }

    show();
    int idx;
    printf("削除する番号を入力してください (戻る:0): ");
    if (scanf("%d", &idx) != 1)
    {
        while (getchar() != '\n')
            ;
        return;
    }

    if (idx == 0)
    {
        printf("メニューに戻ります。\n");
        return;
    }

    if (idx < 1 || idx > rec_count)
    {
        printf("範囲外の番号です。\n");
        return;
    }

    for (int i = idx - 1; i < rec_count - 1; i++)
    {
        recs[i] = recs[i + 1];
    }

    rec_count--;
    printf("記録を削除しました。\n");
}

int main(void)
{
    load();
    int n;
    while (1)
    {
        printf("\n=== RM計算・筋トレ管理アプリ ===\n");
        printf("1. 記録の追加・RM計算\n");
        printf("2. 記録の一覧\n");
        printf("3. データ保存\n");
        printf("4. RM推移（グラフ表示）\n");
        printf("5. 記録の削除\n");
        printf("6. 終了\n");
        printf("番号を選択 >> ");

        if (scanf("%d", &n) != 1)
        {
            printf("不正な入力です（数字を入力してください）\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        if (n < 1 || n > 6)
        {
            printf("範囲外の数字です\n");
            continue;
        }
        switch (n)
        {
        case 1:
            RM();
            break;
        case 2:
            show();
            break;
        case 3:
            save();
            break;
        case 4:
            graph();
            break;
        case 5:
            delete();
            break;
        case 6:
            printf("お疲れ様！！継続は力なり\n");
            return 0;
        }
    }
}
