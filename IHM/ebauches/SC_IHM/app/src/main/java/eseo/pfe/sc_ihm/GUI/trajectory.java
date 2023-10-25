package eseo.pfe.sc_ihm.GUI;


import android.graphics.Canvas;
import android.graphics.Point;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.PriorityQueue;

/**
 * @author dehoux
 * @date 25/10/2023
 */
/*
public class trajectory {
        private Canvas canvas;
        private final int GRID_SIZE;
        private final int WIDTH;
        private final int HEIGHT;
        public TrajectoryCalculator(Canvas canvas, int GRID_SIZE, int WIDTH, int HEIGHT) {
            this.canvas = canvas;
            this.GRID_SIZE = GRID_SIZE;
            this.WIDTH = WIDTH;
            this.HEIGHT = HEIGHT;
        }

        public void drawTrajectory(List<Point> points) {
            if (points.size() == 2) {
                Point point1 = canvas.coords(points.get(0));
                Point point2 = canvas.coords(points.get(1));

                List<Point> path = computePathAStar(point1, point2);

                for (int i = 0; i < path.size() - 1; i++) {
                    Point start = path.get(i);
                    Point end = path.get(i+1);
                    canvas.createLine(start.x, start.y, end.x, end.y, "red", 3, "trajectory");
                }
            }
        }

        public boolean isAccessible(int x, int y) {
            List<Item> items = canvas.findOverlapping(x, y, x + GRID_SIZE, y + GRID_SIZE);
            for (Item item : items) {
                if (!canvas.getTags(item).contains("point") && !canvas.getTags(item).contains("trajectory")) {
                    return false;
                }
            }
            return true;
        }

        public List<Point> neighbors(Point node) {
            int x = node.x;
            int y = node.y;
            Point[] directions = {
                    new Point(0, -GRID_SIZE),
                    new Point(0, GRID_SIZE),
                    new Point(-GRID_SIZE, 0),
                    new Point(GRID_SIZE, 0)
            };

            List<Point> neighborsList = new ArrayList<>();
            for (Point dir : directions) {
                int newX = x + dir.x;
                int newY = y + dir.y;
                if (0 <= newX && newX < WIDTH && 0 <= newY && newY < HEIGHT && isAccessible(newX, newY)) {
                    neighborsList.add(new Point(newX, newY));
                }
            }
            return neighborsList;
        }

        public int heuristic(Point node, Point goal) {
            return Math.abs(node.x - goal.x) + Math.abs(node.y - goal.y);
        }

        public List<Point> generateGridPoints() {
            List<Point> points = new ArrayList<>();
            for (int x = 0; x < WIDTH; x += GRID_SIZE) {
                for (int y = 0; y < HEIGHT; y += GRID_SIZE) {
                    points.add(new Point(x, y));
                }
            }
            return points;
        }

        public List<Point> computePathAStar(Point start, Point goal) {
            start = new Point((start.x / GRID_SIZE) * GRID_SIZE, (start.y / GRID_SIZE) * GRID_SIZE);
            goal = new Point((goal.x / GRID_SIZE) * GRID_SIZE, (goal.y / GRID_SIZE) * GRID_SIZE);

            PriorityQueue<ScoredPoint> openList = new PriorityQueue<>();
            openList.add(new ScoredPoint(0, start));
            HashMap<Point, Point> cameFrom = new HashMap<>();

            List<Point> allGridPoints = generateGridPoints();
            HashMap<Point, Integer> gScore = new HashMap<>();
            HashMap<Point, Integer> fScore = new HashMap<>();

            for (Point point : allGridPoints) {
                gScore.put(point, Integer.MAX_VALUE);
                fScore.put(point, Integer.MAX_VALUE);
            }

            gScore.put(start, 0);
            fScore.put(start, heuristic(start, goal));

            while (!openList.isEmpty()) {
                ScoredPoint currentScoredPoint = openList.poll();
                Point current = currentScoredPoint.point;
                if (current.equals(goal)) {
                    List<Point> path = new ArrayList<>();
                    while (cameFrom.containsKey(current)) {
                        path.add(current);
                        current = cameFrom.get(current);
                    }
                    path.add(start);
                    java.util.Collections.reverse(path);
                    return path;
                }

                for (Point neighbor : neighbors(current)) {
                    int tentativeGScore = gScore.get(current) + heuristic(current, neighbor);
                    if (tentativeGScore < gScore.get(neighbor)) {
                        cameFrom.put(neighbor, current);
                        gScore.put(neighbor, tentativeGScore);
                        fScore.put(neighbor, gScore.get(neighbor) + heuristic(neighbor, goal));
                        if (!containsPoint(openList, neighbor)) {
                            openList.add(new ScoredPoint(fScore.get(neighbor), neighbor));
                        }
                    }
                }
            }

            JOptionPane.showMessageDialog(null, "Il n'y a pas de trajectoire possible", "Erreur", JOptionPane.ERROR_MESSAGE);
            return new ArrayList<>();
        }

        private boolean containsPoint(PriorityQueue<ScoredPoint> queue, Point point) {
            for (ScoredPoint sp : queue) {
                if (sp.point.equals(point)) {
                    return true;
                }
            }
            return false;
        }

        private static class ScoredPoint implements Comparable<ScoredPoint> {
            int score;
            Point point;

            ScoredPoint(int score, Point point) {
                this.score = score;
                this.point = point;
            }

            @Override
            public int compareTo(ScoredPoint other) {
                return Integer.compare(this.score, other.score);
            }


}
*/