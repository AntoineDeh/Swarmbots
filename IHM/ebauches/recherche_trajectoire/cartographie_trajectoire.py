import tkinter as tk
import heapq  # Nous aurons besoin de heapq pour la file de priorité.
from tkinter import messagebox


 

class App:
    def __init__(self, root):
        self.root = root
        self.root.title("Trajectory Planner")

        # Ajoutons des constantes pour la taille de la grille.
        self.WIDTH = 800  # Largeur du canvas
        self.HEIGHT = 800  # Hauteur du canvas
        self.GRID_SIZE = 20  # Taille de la cellule en pixels

        # Canvas
        self.canvas = tk.Canvas(self.root, bg="white", width=800, height=800)
        self.canvas.grid(row=0, column=0, padx=20, pady=self.GRID_SIZE, rowspan=9)

        # Taille uniforme pour tous les boutons
        btn_width = 20
        btn_height = 2

        # Obstacles Buttons
        self.btn_circle = tk.Button(root, text="Circle", command=self.set_obstacle_type_circle, state=tk.NORMAL, width=btn_width, height=btn_height)
        self.btn_circle.grid(row=0, column=1, pady=(0, 0))

        self.btn_rectangle = tk.Button(root, text="Rectangle", command=self.set_obstacle_type_rectangle, state=tk.NORMAL, width=btn_width, height=btn_height)
        self.btn_rectangle.grid(row=1, column=1, pady=(0, 0))

        self.btn_line = tk.Button(root, text="Line", command=self.set_obstacle_type_line, state=tk.NORMAL, width=btn_width, height=btn_height)
        self.btn_line.grid(row=2, column=1, pady=(0, 20))

        # Place Points and Calcul Trajectory Buttons
        self.btn_place_points = tk.Button(root, text="Place Points", command=self.set_place_points_mode, width=btn_width, height=btn_height)
        self.btn_place_points.grid(row=3, column=1, pady=(0, 0))

        self.btn_calcul_trajectory = tk.Button(root, text="Calcul Trajectoire", command=self.draw_trajectory, width=btn_width, height=btn_height)
        self.btn_calcul_trajectory.grid(row=4, column=1, pady=(0, 20))

        # Reset Buttons
        self.btn_reset_obstacles = tk.Button(root, text="Reset Obstacles", command=self.clear_obstacles, width=btn_width, height=btn_height)
        self.btn_reset_obstacles.grid(row=5, column=1, pady=(0, 0))

        self.btn_reset_points = tk.Button(root, text="Reset Points", command=self.reset_points, width=btn_width, height=btn_height)
        self.btn_reset_points.grid(row=6, column=1, pady=(0, 0))

        self.btn_reset_general = tk.Button(root, text="Reset General", command=self.reset_all, width=btn_width, height=btn_height)
        self.btn_reset_general.grid(row=7, column=1, pady=(0, 10))

        # Variables
        self.obstacle_type = None
        self.obstacle_start_point = None
        self.points = []

        # Canvas bindings
        self.canvas.bind("<Button-1>", self.on_canvas_click)

    def on_canvas_click(self, event):
        if self.obstacle_type == "circle":
            if not self.obstacle_start_point:
                self.obstacle_start_point = (event.x, event.y)
            else:
                radius = ((event.x - self.obstacle_start_point[0])**2 + (event.y - self.obstacle_start_point[1])**2)**0.5
                self.canvas.create_oval(self.obstacle_start_point[0]-radius, self.obstacle_start_point[1]-radius,
                                        self.obstacle_start_point[0]+radius, self.obstacle_start_point[1]+radius, fill="black")
                self.obstacle_start_point = None
        elif self.obstacle_type == "rectangle":
            if not self.obstacle_start_point:
                self.obstacle_start_point = (event.x, event.y)
            else:
                self.canvas.create_rectangle(self.obstacle_start_point[0], self.obstacle_start_point[1], event.x, event.y, fill="black")
                self.obstacle_start_point = None
        elif self.obstacle_type == "line":
            if not self.obstacle_start_point:
                self.obstacle_start_point = (event.x, event.y)
            else:
                self.canvas.create_line(self.obstacle_start_point[0], self.obstacle_start_point[1], event.x, event.y, fill="black", width=3)
                self.obstacle_start_point = None
        elif self.obstacle_type == "point":
            if len(self.points) < 2:
                point = self.canvas.create_text(event.x, event.y, text="X", font=("Arial", 16, "bold"), tags="point")
                self.points.append(point)

    def reset_obstacle_type(self):
        self.obstacle_type = None
        self.obstacle_start_point = None
        self.btn_circle.config(state=tk.NORMAL)
        self.btn_rectangle.config(state=tk.NORMAL)
        self.btn_line.config(state=tk.NORMAL)

    def set_obstacle_type_circle(self):
        self.reset_obstacle_type()
        self.obstacle_type = "circle"
        self.btn_circle.config(state=tk.DISABLED)

    def set_obstacle_type_rectangle(self):
        self.reset_obstacle_type()
        self.obstacle_type = "rectangle"
        self.btn_rectangle.config(state=tk.DISABLED)

    def set_obstacle_type_line(self):
        self.reset_obstacle_type()
        self.obstacle_type = "line"
        self.btn_line.config(state=tk.DISABLED)

    def clear_obstacles(self):
        for item in self.canvas.find_all():
            if "point" not in self.canvas.gettags(item) and "trajectory" not in self.canvas.gettags(item):
                self.canvas.delete(item)
        self.reset_obstacle_type()

    def set_place_points_mode(self):
        self.reset_obstacle_type()
        self.obstacle_type = "point"


# Trajectory to modify
#La façon la plus courante de résoudre cela est d'utiliser A*

    def draw_trajectory(self):
        if len(self.points) == 2:
            x1, y1 = self.canvas.coords(self.points[0])
            x2, y2 = self.canvas.coords(self.points[1])
        
        # Calcul du chemin avec A*
        path = self.compute_path_a_star((x1, y1), (x2, y2))
        
        # Dessin du chemin calculé
        for i in range(len(path) - 1):
            x_start, y_start = path[i]
            x_end, y_end = path[i+1]
            self.canvas.create_line(x_start, y_start, x_end, y_end, fill="red", width=3, tags="trajectory")

    
    def is_accessible(self, x, y):
        """Vérifie si une cellule est libre d'obstacle."""
        # Trouve tous les objets qui se chevauchent ou sont à l'intérieur de la cellule spécifiée par (x, y)
        items = self.canvas.find_overlapping(x, y, x + self.GRID_SIZE, y + self.GRID_SIZE)
        # Pour chaque objet dans cette liste d'éléments chevauchants
        for item in items:
            # Si l'objet n'est pas taggé comme un "point" ou une "trajectory"
            if "point" not in self.canvas.gettags(item) and "trajectory" not in self.canvas.gettags(item):
                # Cela signifie que l'objet est un obstacle, donc la cellule n'est pas accessible
                return False
        # Si aucune intersection avec un obstacle n'a été trouvée, la cellule est accessible
        return True


    def neighbors(self, node):
        """Retourne les voisins d'un nœud."""
        x, y = node
        # Les directions possibles pour se déplacer dans la grille
        # (haut, bas, gauche, droite).
        directions = [(0, -self.GRID_SIZE), (0, self.GRID_SIZE), 
                    (-self.GRID_SIZE, 0), (self.GRID_SIZE, 0)]
        # Filtrer les voisins pour qu'ils soient dans les limites du canvas
         # et accessibles (c'est-à-dire sans obstacle).
        neighbors = [(x + dx, y + dy) for dx, dy in directions 
                    if 0 <= x + dx < self.WIDTH and 0 <= y + dy < self.HEIGHT 
                    and self.is_accessible(x + dx, y + dy)]
        return neighbors

    def heuristic(self, node, goal):
        """Distance de Manhattan comme heuristique."""
        # Calculer la distance de Manhattan entre le nœud actuel et le nœud cible.
        return abs(node[0] - goal[0]) + abs(node[1] - goal[1])
    
    def generate_grid_points(self):
        points = []
        for x in range(0, self.WIDTH, self.GRID_SIZE):
            for y in range(0, self.HEIGHT, self.GRID_SIZE):
                points.append((x, y))
        return points

    def compute_path_a_star(self, start, goal):
        # Aligner le début et la fin aux points de la grille les plus proches.
        start = (start[0]//self.GRID_SIZE * self.GRID_SIZE, start[1]//self.GRID_SIZE * self.GRID_SIZE)
        goal = (goal[0]//self.GRID_SIZE * self.GRID_SIZE, goal[1]//self.GRID_SIZE * self.GRID_SIZE)

        open_list = []  # Liste de nœuds à explorer.
        heapq.heappush(open_list, (0, start))  # Commencer par le nœud de départ.
        came_from = {}  # Pour retracer le chemin.

        # Utilisez la fonction generate_grid_points pour obtenir tous les points de la grille
        all_grid_points = self.generate_grid_points()

        # Initialiser les dictionnaires g_score et f_score avec tous les points de la grille
        g_score = {node: float("inf") for node in all_grid_points}
        f_score = {node: float("inf") for node in all_grid_points}
    
        g_score[start] = 0
        f_score[start] = self.heuristic(start, goal)

        while open_list:
            _, current = heapq.heappop(open_list)  # Prendre le nœud avec le score f le plus bas.

            # Si le nœud actuel est le nœud cible, reconstruire le chemin.
            if current == goal:
                path = []
                while current in came_from:
                    path.append(current)
                    current = came_from[current]
                path.append(start)
                path.reverse()
                return path

            # Explorer les voisins du nœud actuel.
            for neighbor in self.neighbors(current):
                # Calculer le score g tentatif pour le voisin.
                tentative_g_score = g_score[current] + self.heuristic(current, neighbor)
            
                # Si ce score tentatif est meilleur que le score actuel du voisin...
                if tentative_g_score < g_score[neighbor]:
                    # Mise à jour du chemin et des scores pour ce voisin.
                    came_from[neighbor] = current
                    g_score[neighbor] = tentative_g_score
                    f_score[neighbor] = g_score[neighbor] + self.heuristic(neighbor, goal)
                    # Ajouter le voisin à la liste ouverte s'il n'y est pas déjà.
                    if neighbor not in [item[1] for item in open_list]:
                        heapq.heappush(open_list, (f_score[neighbor], neighbor))

        # Si la boucle se termine sans avoir trouvé de chemin, affichez le message d'erreur.
        messagebox.showerror("Erreur", "Il n'y a pas de trajectoire possible")
        
        # Retourner une liste vide si aucun chemin n'est trouvé.
        return []



    def clear_trajectory(self):
        for item in self.canvas.find_withtag("trajectory"):
            self.canvas.delete(item)

    def reset_points(self):
        for item in self.canvas.find_withtag("point"):
            self.canvas.delete(item)
        self.points = []
        self.clear_trajectory()

    def reset_all(self):
        self.clear_obstacles()
        self.reset_points()
        self.clear_trajectory()
        self.reset_obstacle_type()

if __name__ == "__main__":
    root = tk.Tk()
    app = App(root)
    root.mainloop()
