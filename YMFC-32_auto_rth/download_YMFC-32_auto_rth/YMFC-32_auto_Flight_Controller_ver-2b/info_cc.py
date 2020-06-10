#Question 5

#renvoit la norme d'une matrice

def get_norme(mat):
    ordre = len(mat[0])
    maxi = 0 
    for l in range (0, ordre):
        sum = 0
        for c in range (0, ordre):
            sum += abs(mat[l][c])
        maxi = max(maxi, sum)
    return maxi 

matrice  = [[10, -32, 3], [3, 4, 5], [7, 9, 3]]
print(get_norme(matrice))
    
    

