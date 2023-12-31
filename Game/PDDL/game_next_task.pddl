(define (problem GameNextTask)
	(:domain GameDomain)
	(:objects
		cl1 - cleaner
		t1 - trash
		s1 s2 s3 - stone
		c11 c12 c13 c14 c15 c16 c17 c18 - cell
		c21 c22 c23 c24 c25 c26 c27 c28 - cell
		c31 c32 c33 c34 c35 c36 c37 c38 - cell
		c41 c42 c43 c44 c45 c46 c47 c48 - cell
	)
	(:init
		(= (row_cell c11) 1) (= (col_cell c11) 1)
		(= (row_cell c12) 1) (= (col_cell c12) 2)
		(= (row_cell c13) 1) (= (col_cell c13) 3)
		(= (row_cell c14) 1) (= (col_cell c14) 4)
		(= (row_cell c15) 1) (= (col_cell c15) 5)
		(= (row_cell c16) 1) (= (col_cell c16) 6)
		(= (row_cell c17) 1) (= (col_cell c17) 7)
		(= (row_cell c18) 1) (= (col_cell c18) 8)
		(= (row_cell c21) 2) (= (col_cell c21) 1)
		(= (row_cell c22) 2) (= (col_cell c22) 2)
		(= (row_cell c23) 2) (= (col_cell c23) 3)
		(= (row_cell c24) 2) (= (col_cell c24) 4)
		(= (row_cell c25) 2) (= (col_cell c25) 5)
		(= (row_cell c26) 2) (= (col_cell c26) 6)
		(= (row_cell c27) 2) (= (col_cell c27) 7)
		(= (row_cell c28) 2) (= (col_cell c28) 8)
		(= (row_cell c31) 3) (= (col_cell c31) 1)
		(= (row_cell c32) 3) (= (col_cell c32) 2)
		(= (row_cell c33) 3) (= (col_cell c33) 3)
		(= (row_cell c34) 3) (= (col_cell c34) 4)
		(= (row_cell c35) 3) (= (col_cell c35) 5)
		(= (row_cell c36) 3) (= (col_cell c36) 6)
		(= (row_cell c37) 3) (= (col_cell c37) 7)
		(= (row_cell c38) 3) (= (col_cell c38) 8)
		(= (row_cell c41) 4) (= (col_cell c41) 1)
		(= (row_cell c42) 4) (= (col_cell c42) 2)
		(= (row_cell c43) 4) (= (col_cell c43) 3)
		(= (row_cell c44) 4) (= (col_cell c44) 4)
		(= (row_cell c45) 4) (= (col_cell c45) 5)
		(= (row_cell c46) 4) (= (col_cell c46) 6)
		(= (row_cell c47) 4) (= (col_cell c47) 7)
		(= (row_cell c48) 4) (= (col_cell c48) 8)
		(stoneExist s1)
		(stoneExist s2)
		(stoneExist s3)
		(free c11)	(free c12)	(free c13)	(free c14)	(free c15)	(free c16)	(free c17)	(free c18)	
		(occupied c21 s2)(occupied c22 s1)(free c23)	(free c24)	(free c25)	(free c26)	(free c27)	(free c28)	
		(free c31)	(free c32)	(free c33)	(occupied c34 s3)(free c35)	(free c36)	(free c37)	(free c38)	
		(free c41)	(free c42)	(free c43)	(free c44)	(free c45)	(free c46)	(free c47)	(free c48)	
		(ready cl1)
		(= (row_cleaner cl1) 4)
		(= (col_cleaner cl1) 2)
		(= (row_trash t1) 4)
		(= (col_trash t1) 1)

		(= (totalEnergyCost) 0)
	)
	(:goal (and
		(stoneRemoved s1)
		(stoneRemoved s2)
		(stoneRemoved s3)
	))
	(:metric minimize (totalEnergyCost))
)