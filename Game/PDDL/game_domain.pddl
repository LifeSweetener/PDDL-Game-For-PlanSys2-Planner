(define (domain GameDomain)
	(:requirements :strips :typing :equality :fluents)
	(:types
		cleaner								;; уборщик
		stone								;; булыжник
		trash								;; мусорка
		cell								;; клетка локации
	)
	(:predicates 
		(occupied ?c - cell ?s - stone)					;; клетка локации загорожена булыжником
		(free ?c - cell)						;; клетка свободна для перемещения
		(loaded ?cl - cleaner)						;; уборщик нагружен большим камнем и не может взять ещё
		(ready ?cl - cleaner)						;; уборщик может взять с собой один булыжник
		(stonePicked ?s - stone ?cl - cleaner)				;; камень подобран
		(stoneRemoved ?s - stone)					;; камень выброшен
		(stoneExist ?s - stone)						;; камень не выброшен
	)
	(:functions
		(row_cleaner ?cl - cleaner)
		(row_trash ?t - trash)
		(row_cell ?c - cell)
		
		(col_cleaner ?cl - cleaner)
		(col_trash ?t - trash)
		(col_cell ?c - cell)
		
		(totalEnergyCost)
	)
	
	;;============ПЕРЕМЕЩЕНИЕ БЕЗ БУЛЫЖНИКА:======================================
	;; Пойти направо:
	(:action moveRight
		:parameters (?cl - cleaner ?c - cell)
		:precondition (and
			(= (col_cell ?c) (+ (col_cleaner ?cl) 1))		;; целевая клетка находится рядом правее
			(= (row_cell ?c) (row_cleaner ?cl))
			(free ?c)
			(ready ?cl)
		)
		:effect (and
			(increase (col_cleaner ?cl) 1)
			(increase (totalEnergyCost) 1)
		)
	)
	
	;; Пойти налево:
	(:action moveLeft
		:parameters (?cl - cleaner ?c - cell)
		:precondition (and
			(= (col_cell ?c) (- (col_cleaner ?cl) 1))		;; целевая клетка находится рядом левее
			(= (row_cell ?c) (row_cleaner ?cl))
			(free ?c)
			(ready ?cl)
		)
		:effect (and
			(decrease (col_cleaner ?cl) 1)
			(increase (totalEnergyCost) 1)
		)
	)
	
	;; Пойти вперёд (вверх):
	(:action moveUp
		:parameters (?cl - cleaner ?c - cell)
		:precondition (and
			(= (row_cell ?c) (- (row_cleaner ?cl) 1))		;; целевая клетка находится рядом выше
			(= (col_cell ?c) (col_cleaner ?cl))
			(free ?c)
			(ready ?cl)
		)
		:effect (and
			(decrease (row_cleaner ?cl) 1)
			(increase (totalEnergyCost) 1)
		)
	)
	
	;; Пойти назад (вниз):
	(:action moveDown
		:parameters (?cl - cleaner ?c - cell)
		:precondition (and
			(= (row_cell ?c) (+ (row_cleaner ?cl) 1))		;; целевая клетка находится рядом ниже
			(= (col_cell ?c) (col_cleaner ?cl))
			(free ?c)						;; клетка свободна
			(ready ?cl)
		)
		:effect (and
			(increase (row_cleaner ?cl) 1)
			(increase (totalEnergyCost) 1)
		)
	)
	
	;;============ПЕРЕМЕЩЕНИЕ С БУЛЫЖНИКОМ:======================================
	;; Пойти направо:
	(:action moveRightLoaded
		:parameters (?cl - cleaner ?c - cell)
		:precondition (and
			(= (col_cell ?c) (+ (col_cleaner ?cl) 1))		;; целевая клетка находится рядом правее
			(= (row_cell ?c) (row_cleaner ?cl))
			(free ?c)
			(loaded ?cl)
		)
		:effect (and
			(increase (col_cleaner ?cl) 1)
			(increase (totalEnergyCost) 2)
		)
	)
	
	;; Пойти налево:
	(:action moveLeftLoaded
		:parameters (?cl - cleaner ?c - cell)
		:precondition (and
			(= (col_cell ?c) (- (col_cleaner ?cl) 1))		;; целевая клетка находится рядом левее
			(= (row_cell ?c) (row_cleaner ?cl))
			(free ?c)
			(loaded ?cl)
		)
		:effect (and
			(decrease (col_cleaner ?cl) 1)
			(increase (totalEnergyCost) 2)
		)
	)
	
	;; Пойти вперёд (вверх):
	(:action moveUpLoaded
		:parameters (?cl - cleaner ?c - cell)
		:precondition (and
			(= (row_cell ?c) (- (row_cleaner ?cl) 1))		;; целевая клетка находится рядом выше
			(= (col_cell ?c) (col_cleaner ?cl))
			(free ?c)
			(loaded ?cl)
		)
		:effect (and
			(decrease (row_cleaner ?cl) 1)
			(increase (totalEnergyCost) 2)
		)
	)
	
	;; Пойти назад (вниз):
	(:action moveDownLoaded
		:parameters (?cl - cleaner ?c - cell)
		:precondition (and
			(= (row_cell ?c) (+ (row_cleaner ?cl) 1))		;; целевая клетка находится рядом ниже
			(= (col_cell ?c) (col_cleaner ?cl))
			(free ?c)						;; клетка свободна
			(loaded ?cl)
		)
		:effect (and
			(increase (row_cleaner ?cl) 1)
			(increase (totalEnergyCost) 2)
		)
	)
	
	;;===============================================================================
	
	;; Взять камень справа:
	(:action pickRightStone
		:parameters (?cl - cleaner ?c - cell ?s - stone)
		:precondition (and
			(= (col_cell ?c) (+ (col_cleaner ?cl) 1))		;; целевая клетка находится рядом правее
			(= (row_cell ?c) (row_cleaner ?cl))
			(occupied ?c ?s)					;; клетка с камнем
			(ready ?cl)						;; руки уборщика не заняты
			(stoneExist ?s)						;; булыжник не выброшен
		)
		:effect (and
			(free ?c)
			(loaded ?cl)
			(not (ready ?cl))
			(not (occupied ?c ?s))
			(increase (col_cleaner ?cl) 1)
			(stonePicked ?s ?cl)
			(increase (totalEnergyCost) 2)
		)
	)
	
	;; Взять камень слева:
	(:action pickLeftStone
		:parameters (?cl - cleaner ?c - cell ?s - stone)
		:precondition (and
			(= (col_cell ?c) (- (col_cleaner ?cl) 1))		;; целевая клетка находится рядом левее
			(= (row_cell ?c) (row_cleaner ?cl))
			(occupied ?c ?s)					;; клетка с камнем
			(ready ?cl)						;; руки уборщика не заняты
			(stoneExist ?s)						;; булыжник не выброшен
		)
		:effect (and
			(free ?c)
			(loaded ?cl)
			(not (ready ?cl))
			(not (occupied ?c ?s))
			(decrease (col_cleaner ?cl) 1)
			(stonePicked ?s ?cl)
			(increase (totalEnergyCost) 2)
		)
	)
	
	;;===============================================================================
	;; Выбросить в помойку слева:
	(:action throwStoneLeft
		:parameters (?cl - cleaner ?s - stone ?t - trash)
		:precondition (and
			(= (col_trash ?t) (- (col_cleaner ?cl) 1))			;; помойка находится рядом левее
			(= (row_trash ?t) (row_cleaner ?cl))
			(stonePicked ?s ?cl)						;; у уборщика ?cl камень ?s
		)
		:effect (and
			(not (loaded ?cl))
			(ready ?cl)
			(stoneRemoved ?s)
			(not (stoneExist ?s))
			(not (stonePicked ?s ?cl))
			(increase (totalEnergyCost) 2)
		)
	)
	
	;; Выбросить в помойку справа:
	(:action throwStoneRight
		:parameters (?cl - cleaner ?s - stone ?t - trash)
		:precondition (and
			(= (col_trash ?t) (+ (col_cleaner ?cl) 1))			;; помойка находится рядом правее
			(= (row_trash ?t) (row_cleaner ?cl))			
			(stonePicked ?s ?cl)						;; у уборщика ?cl камень ?s
		)
		:effect (and
			(not (loaded ?cl))
			(ready ?cl)
			(stoneRemoved ?s)
			(not (stoneExist ?s))
			(not (stonePicked ?s ?cl))
			(increase (totalEnergyCost) 2)
		)
	)
)
