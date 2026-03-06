% Pulizia ambiente
clear; close all; clc;

% Leggo il file CSV in una tabella
scriptDir = fileparts(mfilename('fullpath'));
data = readtable(fullfile(scriptDir, 'traiettorie_tempo.csv'));

% Estraggo le colonne
time = data.Time;   % Tempo
star = data.Star;   % Numero della stella
x = data.X;         % Coordinata X
y = data.Y;         % Coordinata Y

% Preparo il grafico
figure;
hold on;
colors = lines(7);  % Genera 7 colori distinti

% Plotto la traiettoria di ogni stella
for i = 1:7
    idx = (star == i);
    % Traccio la traiettoria per la stella i
    plot(x(idx), y(idx), 'Color', colors(i,:), 'LineWidth', 2, ...
         'DisplayName', sprintf('Stella %d', i));
end

xlabel('X');
ylabel('Y');
title('Traiettorie finali delle Stelle');
legend('show');
grid on;

% Analisi collisione: distanza minima globale tra tutte le coppie di stelle
numStars = max(star);
uniqueTimes = unique(time);
nSteps = numel(uniqueTimes);
X = NaN(nSteps, numStars);
Y = NaN(nSteps, numStars);

for i = 1:numStars
    idx = (star == i);
    ti = time(idx);
    xi = x(idx);
    yi = y(idx);
    [~, loc] = ismember(ti, uniqueTimes);
    X(loc, i) = xi;
    Y(loc, i) = yi;
end

minDist = inf;
starA = -1;
starB = -1;
tMin = NaN;
posA = [NaN, NaN];
posB = [NaN, NaN];

for s = 1:nSteps
    for i = 1:numStars
        for j = i+1:numStars
            if any(isnan([X(s,i), Y(s,i), X(s,j), Y(s,j)]))
                continue;
            end
            dist = hypot(X(s,i) - X(s,j), Y(s,i) - Y(s,j));
            if dist < minDist
                minDist = dist;
                starA = i;
                starB = j;
                tMin = uniqueTimes(s);
                posA = [X(s,i), Y(s,i)];
                posB = [X(s,j), Y(s,j)];
            end
        end
    end
end

% Marker grafico del punto di minima distanza
plot([posA(1), posB(1)], [posA(2), posB(2)], 'k--', 'LineWidth', 1.5, ...
     'DisplayName', 'Min distance segment');
scatter(posA(1), posA(2), 60, 'k', 'filled', 'DisplayName', sprintf('Star %d min dist', starA));
scatter(posB(1), posB(2), 60, 'k', 'filled', 'DisplayName', sprintf('Star %d min dist', starB));
legend('show');

fprintf('Distanza minima: %.10f\n', minDist);
fprintf('Coppia di stelle: %d e %d\n', starA, starB);
fprintf('Tempo: %.10f\n', tMin);

hold off;
