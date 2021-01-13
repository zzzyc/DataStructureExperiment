/*Written by zycsolego*/
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<queue>
#include<iostream>
#include<stack> 

using namespace std;

const int MAX_SCENIC = 16;
const int MAX_ROAD = 30;
const int MAX_CROSS = 50;
const int MAX_X = 1001;
const int MAX_Y = 1001;

int exist[MAX_X][MAX_Y];

struct Point{
	int u, v;
	bool operator < (const Point &A) const {
		return v > A.v;
	}
};

struct Cross{
    string name;
    string roadx;
    string roady;
    int x, y;
}C[MAX_CROSS];
int c_count;

struct Scenic{
    int sign; //S1, S2...
	string name;
	string road;
	int road_pos;
	int x, y;
}S[MAX_SCENIC];
int s_count;

struct Road{
    int sign;
    string name;
    int distance;
    int dirx;
    int Scenic_num[MAX_SCENIC];
    int Scenic_cnt;
}R[MAX_ROAD];
int r_count;

struct Dij{
	static const int N = 110;
	static const int M = 20010;
	int h[N], e[M], w[M], ne[M], idx;
	int dist[N][N];
	int st[N][N];
	int pre[N][N];

	void init() {
		memset(h, -1, sizeof h);
		idx = 0;
	}
	
	void add(int a, int b, int c) {
		e[idx] = b, w[idx] = c, ne[idx] = h[a], h[a] = idx++;
        e[idx] = a, w[idx] = c, ne[idx] = h[b], h[b] = idx++;
	}

	void get_All_Edges() {
		for(int i = 1; i <= s_count; i++) 
			for(int j = 1; j <= c_count; j++) {
                if(S[i].road == C[j].roadx) {
                    int a = i, b = s_count + j;
                    add(a, b, abs(S[i].y - C[j].y));
                }
                else if(S[i].road == C[j].roady) {
                    int a = i, b = s_count + j;
                    add(a, b, abs(S[i].x - C[j].x));
                }
            }

        for(int i = 1; i <= s_count; i++)
            for(int j = 1; j <= s_count; j++) {
                if(i == j) continue;
                if(S[i].road == S[j].road) {
                    if(S[i].x == S[j].x) add(i, j, abs(S[i].y - S[j].y));
                    else add(i, j, abs(S[i].x - S[j].x));
                }
            }
	}
	
	void dijkstra(int fir) {
		memset(pre[fir], 0, sizeof pre[fir]);
		memset(dist[fir], 0x3f, sizeof dist[fir]);
		memset(st[fir], 0, sizeof st[fir]);
		dist[fir][fir] = 0;
		priority_queue<Point> q;
		q.push({fir, 0});

		while(!q.empty()) {
			Point t = q.top(); q.pop();
			int u = t.u;
			if(st[fir][u]) continue;
			st[fir][u] = 1;

			for(int i = h[u]; i != -1; i = ne[i]) {
				int v = e[i];
				if(dist[fir][v] > dist[fir][u] + w[i]) {
					dist[fir][v] = dist[fir][u] + w[i];
					q.push({v, dist[fir][v]});
					pre[fir][v] = u;
				}
			}
		}
	}
	
	void get_All_dist() {
		for(int i = 1; i <= s_count; i++) dijkstra(i);
	}
};

void out_space(int n) {
	for(int i = 0; i < n; i++) cout << " ";
}

int get_length(int n) {
	int res = 0;
	while(n) {
		++res;
		n /= 10;
	}
	return res;
}

int get_Scenic(int id) {
	printf("请输入%c景点的名称：\n", id + 65);
	string A_name; cin >> A_name;
	for(int i = 1; i <= s_count; i++) 
		if(S[i].name == A_name) return i;
	return 0;
}

int get_Road(int id) {
	for(int i = 1; i <= r_count; i++)
		if(S[id].road == R[i].name) return i;
	return 0;
}

void out_Scenic(int n) {
	printf("景点标志                景点名称                景点地址\n");
	for(int i = 1; i <= n; i++) {
		printf("S%d", S[i].sign);
		out_space(24 - get_length(S[i].sign) - 1);
		printf("%s", S[i].name.c_str());
		out_space(24 - S[i].name.length());
		printf("%s%d号\n\n", S[i].road.c_str(), S[i].road_pos);
	}
}

void out_Road(int n) {
	printf("道路标志                距离(m)                 道路名                沿路景点\n");
	for(int i = 1; i <= n; i++) {
		printf("R%d", R[i].sign);
		out_space(24 - get_length(S[i].sign) - 1);
		printf("%d", 12 * R[i].distance);
		out_space(24 - get_length(R[i].distance));
		printf("%s", R[i].name.c_str());
		out_space(22 - R[i].name.length());
		for(int j = 1; j <= R[i].Scenic_cnt; j++) { 
			printf("S%d", R[i].Scenic_num[j]);
			if(j < R[i].Scenic_cnt) printf(","); 
		}
		printf("\n\n");
	}
}

void input() {
	freopen("input.in", "r", stdin);
	printf("请输入景点的数量：\n"); cin >> s_count;
	for(int i = 1; i <= s_count; i++) {
		S[i].sign = i;
		printf("请输入第%d个景点的名称：\n", i); cin >> S[i].name;
		printf("请输入第%d个景点所在路：\n", i); cin >> S[i].road;
		printf("请输入第%d个景点在路的具体位置：\n", i); cin >> S[i].road_pos;
	}

	printf("请输入道路的数量：\n"); scanf("%d", &r_count);
	for(int i = 1; i <= r_count; i++) {
		R[i].sign = i;
		printf("请输入第%d条道路名：\n", i); cin >> R[i].name;
		printf("请输入这条路是横向(1)还是纵向(0)：\n"); cin >> R[i].dirx;
		printf("请输入该道路距离%s轴的距离：\n", R[i].dirx == 1 ? "x" : "y"); cin >> R[i].distance;
		
		R[i].Scenic_cnt = 0;
		for(int j = 1; j <= s_count; j++) {
			if(S[j].road == R[i].name) {
				++R[i].Scenic_cnt;
				R[i].Scenic_num[R[i].Scenic_cnt] = S[j].sign;
				if(R[i].dirx) exist[R[i].distance][S[j].road_pos] = j;
				else exist[S[j].road_pos][R[i].distance] = j;
			} 
		}
		sort(R[i].Scenic_num + 1, R[i].Scenic_num + 1 + R[i].Scenic_cnt);
	}
	
	for(int i = 1; i <= s_count; i++) {
		int Road_num = get_Road(i);
		int dirx = R[Road_num].dirx;
		if(dirx) S[i].y = R[Road_num].distance, S[i].x = S[i].road_pos;
		else S[i].x = R[Road_num].distance, S[i].y = S[i].road_pos;
	}

    c_count = 0;
    for(int i = 1; i <= r_count; i++) 
        for(int j = i + 1; j <= r_count; j++) 
            if(R[i].dirx + R[j].dirx == 1) {
                
                ++c_count;
                if(R[i].dirx) {
                    C[c_count].roadx = R[i].name;
                    C[c_count].roady = R[j].name;
                    C[c_count].name = R[i].name + "与" + R[j].name + "交叉口";
                }
                else {
                	C[c_count].roadx = R[j].name;
                    C[c_count].roady = R[i].name;
                    C[c_count].name = R[j].name + "与" + R[i].name + "交叉口";
				}
				
            }
}

void query_Assign_Scenic() {
	int To_num = get_Scenic(0);
	if(!To_num) {
		printf("该景点不存在！\n");
		return ; 
	} 
	
	printf("景点标志                景点名称                景点地址\n");
	printf("S%d", S[To_num].sign);
	out_space(24 - get_length(S[To_num].sign) - 1);
	printf("%s", S[To_num].name.c_str());
	out_space(24 - S[To_num].name.length());
	printf("%s%d号\n\n", S[To_num].road.c_str(), S[To_num].road_pos);

}

void query_Adjoin_Scenic() {
	int To_num = get_Scenic(0);
	if(!To_num) {
		printf("该景点不存在！\n");
		return ; 
	} 
	
	int cnt = 0;
	for(int i = 1; i <= s_count; i++) {
		if(i == To_num) continue;
		if(S[To_num].road == S[i].road) ++cnt;
	} 
	
	if(!cnt) {
		printf("该景点无邻接景点！\n");
		return ;
	}
	
	printf("该景点有%d个邻接景点\n", cnt);
	int tcnt = 0;
	for(int i = 1; i <= s_count; i++) {
		if(i == To_num) continue;
		if(S[To_num].road == S[i].road) {
			++tcnt;
			printf("%s%c", S[i].name.c_str(), " \n"[tcnt == cnt]);
		}
	}  
} 

void Best_NowToAssign_Scenic() {
	int To_numA = get_Scenic(0);
	if(!To_numA) {
		printf("该景点不存在！\n");
		return ; 
	} 
	
	int To_numB = get_Scenic(1);
	if(!To_numB) {
		printf("该景点不存在！\n");
		return ; 
	} 
	
	if(To_numA == To_numB) {
		printf("输入的两个景点相同！\n");
		return ;
	}
	
	Dij D;
	D.init();
	D.get_All_Edges();
	D.get_All_dist();
	printf("从%s到%s的最短路距离为：%d(m)\n", S[To_numA].name.c_str(), S[To_numB].name.c_str(), 12 * D.dist[To_numA][To_numB]);
	printf("路线为：\n");
	stack<int> stk;
	int now = To_numB;
	while(now) {
		stk.push(now);
		now = D.pre[To_numA][now];
	}

	int sz = stk.size();
	printf("sz:%d\n", sz);
	int cnt = 0;
	while(!stk.empty()) {
		++cnt;
        int t = stk.top();
		printf("%s", t > s_count ? C[t - s_count].name.c_str() : S[t].name.c_str());
		if(cnt < sz) printf("---->");
		else printf("\n");
		stk.pop();
	}
}

void Best_NowToAll_Scenic() {
int To_numA = get_Scenic(0);
	if(!To_numA) {
		printf("该景点不存在！\n");
		return ; 
	} 
	
	Dij D;
	D.init();
	D.get_All_Edges();
	D.dijkstra(To_numA);
	for(int To_numB = 1; To_numB <= s_count; To_numB++) { 
		if(To_numA == To_numB) continue;
		printf("从%s到%s的最短路距离为：%d(m)\n", S[To_numA].name.c_str(), S[To_numB].name.c_str(), 12 * D.dist[To_numA][To_numB]);
		printf("路线为：\n");
		stack<int> stk;
		int now = To_numB;
		while(now) {
			stk.push(now);
			now = D.pre[To_numA][now];
		}
	
		int sz = stk.size();
		int cnt = 0;
		while(!stk.empty()) {
			++cnt;
	        int t = stk.top();
			printf("%s", t > s_count ? C[t - s_count].name.c_str() : S[t].name.c_str());
			if(cnt < sz) printf("---->");
			else printf("\n");
			stk.pop();
		} printf("\n");
	} 
}

void Modify_Assign_Scenic() {
	int To_numA = get_Scenic(0);
	if(!To_numA) {
		printf("该景点不存在！\n");
		return ; 
	} 

	while(true) {
		printf("请输入你要修改的信息类型：\n");
		printf("(1). 景点名称\n");
		printf("(2). 景点地址");
		printf("(3). 修改完毕！退出修改！\n");
		int choice; cin >> choice;
		if(choice == 1) {
			printf("请输入修改后的景点名称：\n");
			cin >> S[To_numA].name;
			printf("修改成功！\n");
		}
		else if(choice == 2) {
			printf("请输入修改后的景点所在道路名：\n");
			string tname;
			cin >> tname;
			int flag = 0;
			for(int i = 1; i <= r_count; i++) {
				if(tname == R[i].name) {
					S[To_numA].road = tname;
					flag = 1;
					break;
				}
			}
			if(!flag) printf("该道路不存在！\n");
			else {
				printf("景点所在道路修改成功！\n");
				printf("请输入修改后的景点在所在道路的位置：\n");
				cin >> S[To_numA].road_pos;
			}
		}
		else break;
	}
}

void Show_Panoramagram(int n = 70, int m = 60) {
	//get下最右下标 
	int r = m;
	for(int i = 1; i <= n; i++)
		for(int j = 1; j <= m; j++) 
			if(exist[i][j]) r = max(r, j + 30);
	
	for(int j = 0; j <= r + 1; j++) printf("-"); printf("\n");
	for(int i = 1; i <= n; i++, printf("\n")) { 
		printf("|");
		
		int j;
		for(j = 0; j <= m; j++) {
			if(exist[i][j]) {
				int p = exist[i][j];
				printf("·%s", S[p].name.c_str());
				printf("(%d, %d)", i, j);
				j += S[p].name.length() + 6 + get_length(i) + get_length(j) - 1;
			}
			else printf(" ");
		}
		for(; j < r; j++) printf(" ");
		printf("|");
	} 
	for(int j = 0; j <= r + 1; j++) printf("-"); printf("\n");
}

const int MAXV = 1 << MAX_SCENIC;
int f[MAXV][MAX_SCENIC];
int TSP_pre[MAXV][MAX_SCENIC];
void TSP() {
	int To_numA = get_Scenic(0);
	if(!To_numA) {
		printf("该景点不存在！\n");
		return ; 
	} 
	
	Dij D;
	D.init();
	D.get_All_Edges();
	D.get_All_dist();
	memset(f, 0x3f, sizeof f);
	memset(TSP_pre, 0, sizeof TSP_pre);
	int now = (1 << (To_numA - 1));
	TSP_pre[now][To_numA] = 0;
	f[now][To_numA - 1] = 0;
	for(int i = now; i < (1 << s_count); i++) {
		i |= now;
		
		//枚举当前状态的尾点，以及下一个点 
		for(int j = 0; j < s_count; j++) {
			if(!(i >> j & 1)) continue;
			
			int a = j + 1;
			for(int k = 0; k < s_count; k++) {
				if(i >> k & 1) continue;
				
				int b = k + 1;
				if(f[i + (1 << k)][k] > f[i][j] + D.dist[a][b]) { 
					f[i + (1 << k)][k] = f[i][j] + D.dist[a][b];
					TSP_pre[i + (1 << k)][b] = a;
				} 
			}
		} 
	}
	
	int val = 0x3f3f3f3f, last = 0;
	for(int i = 0; i < s_count; i++) {
		if(i == To_numA - 1) continue;
		if(val > f[(1 << s_count) - 1][i]) {
			val = f[(1 << s_count) - 1][i];
			last = i;
		}
	}
	
	++last;
	stack<int> stk;
	stk.push(last);
	printf("最佳浏览路径的总路程为：%d(m)\n", val);
	val = (1 << s_count) - 1;
	while(TSP_pre[val][last]) {
		stk.push(TSP_pre[val][last]);
		int tval = val - (1 << (last - 1));
		last = TSP_pre[val][last];
		val = tval;
	}
	
	printf("路线为：\n");
	int sz = stk.size();
	int cnt = 0;
	while(!stk.empty()) {
		++cnt;
		printf("%s", S[stk.top()].name.c_str());
		if(cnt < sz) printf("---->");
		else printf("\n");
		stk.pop();
	}
}

void main_operation() {
	while(true) {
		
		printf("--------------------------------------------------\n");
		printf("| 1. A景点信息查询                               |\n");
		printf("| 2. A景点的邻接景点信息查询                     |\n");
		printf("| 3. 给出从A景点到B景点的最佳路线                |\n");
		printf("| 4. 给出从A景点到所有景点的最佳路线             |\n"); 
		printf("| 5. 修改A景点信息                               |\n");
		printf("| 6. 显示全景图                                  |\n");
		printf("| 7. 显示所有景点信息                            |\n");
		printf("| 8. 显示所有道路信息                            |\n");
		printf("| 9. 由A景点出发浏览所有景点的最佳路线           |\n");
		printf("| 0. 退出                                        |\n");
		printf("--------------------------------------------------\n");
		printf("请输入相应的操作序号进行相应的操作：\n");
		
		int choice; cin >> choice;
		switch(choice) {
			case 1 : query_Assign_Scenic(); break;
			case 2 : query_Adjoin_Scenic(); break;
			case 3 : Best_NowToAssign_Scenic(); break;
			case 4 : Best_NowToAll_Scenic(); break;
			case 5 : Modify_Assign_Scenic(); break;
			case 6 : Show_Panoramagram(); break;
			case 7 : out_Scenic(s_count); break;
			case 8 : out_Road(r_count); break;
			case 9 : TSP(); break;
			case 0 : return ;
			default : printf("输入错误！请重新输入！\n"); break;
		}
	}
}
int main()
{
    input();
    main_operation();
    printf("退出成功！\n");
	return 0;
}
