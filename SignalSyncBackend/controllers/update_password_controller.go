package controllers

import (
	"database/sql"
	"net/http"
	"signalsync/backend/database"
	"signalsync/backend/services"

	"github.com/gin-gonic/gin"
)

func UpdatePasswordController(db *sql.DB) gin.HandlerFunc {
	return func(c *gin.Context) {
		var req database.SessionVerification

		if !services.ValidateRequestBodyService(&req, c) {
			return
		}

		if !services.DeleteUserService(db, &req) {
			c.IndentedJSON(http.StatusBadRequest, gin.H{"error": "Invalid Credentials"})
			return
		}

		c.IndentedJSON(http.StatusOK, gin.H{"success": "Delete Successful"})

	}
}
