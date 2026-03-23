package controllers

import (
	"database/sql"
	"net/http"
	"signalsync/backend/database"
	"signalsync/backend/services"

	"github.com/gin-gonic/gin"
)

func ValidateLogin(db *sql.DB) gin.HandlerFunc {
	return func(c *gin.Context) {
		var req database.Login

		if !services.ValidateRequestBodyService(&req, c) {
			return
		}
		err, api_key := services.ValidateLoginService(db, &req)
		if !err {
			c.IndentedJSON(http.StatusBadRequest, gin.H{"error": "Invalid Credentials"})
			return
		}

		c.String(http.StatusOK, api_key)
	}
}
