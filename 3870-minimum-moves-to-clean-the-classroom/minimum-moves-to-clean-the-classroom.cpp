class Solution {
public:
    int minMoves(vector<string>& classroom, int energy) {
        
        int m = classroom.size();
        int n = classroom[0].size();

        int sr, sc;
        vector<pair<int, int>> litter;

        // Find start and litter
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                
                if (classroom[i][j] == 'S') {
                    sr = i;
                    sc = j;
                }

                if (classroom[i][j] == 'L') {
                    litter.push_back({i, j});
                }
            }
        }

        int L = litter.size();
        int target = (1 << L) - 1;

        // If no litter
        if (L == 0) return 0;

        // Litter index for every cell
        vector<vector<int>> id(m, vector<int>(n, -1));

        for (int i = 0; i < L; i++) {
            id[litter[i].first][litter[i].second] = i;
        }

        /*
        best[r][c][mask] =
        maximum energy with which we reached this state
        */
        vector<vector<vector<int>>> best(
            m,
            vector<vector<int>>(n, vector<int>(1 << L, -1))
        );

        // row, col, energy, mask
        queue<tuple<int, int, int, int>> q;

        q.push({sr, sc, energy, 0});
        best[sr][sc][0] = energy;

        int moves = 0;

        int dr[] = {-1, 1, 0, 0};
        int dc[] = {0, 0, -1, 1};

        while (!q.empty()) {

            int size = q.size();

            // All states at the same number of moves
            while (size--) {

                auto [r, c, e, mask] = q.front();
                q.pop();

                if (mask == target)
                    return moves;

                // Try all 4 directions
                for (int k = 0; k < 4; k++) {

                    int nr = r + dr[k];
                    int nc = c + dc[k];

                    // Outside grid
                    if (nr < 0 || nr >= m || nc < 0 || nc >= n)
                        continue;

                    // Obstacle
                    if (classroom[nr][nc] == 'X')
                        continue;

                    // Need energy to move
                    if (e == 0)
                        continue;

                    int newEnergy = e - 1;
                    int newMask = mask;

                    // Collect litter
                    if (id[nr][nc] != -1) {
                        newMask |= (1 << id[nr][nc]);
                    }

                    // Reset energy
                    if (classroom[nr][nc] == 'R') {
                        newEnergy = energy;
                    }

                    // We already reached this state with
                    // equal or greater energy
                    if (best[nr][nc][newMask] >= newEnergy)
                        continue;

                    best[nr][nc][newMask] = newEnergy;

                    q.push({
                        nr,
                        nc,
                        newEnergy,
                        newMask
                    });
                }
            }

            moves++;
        }

        return -1;
    }
};